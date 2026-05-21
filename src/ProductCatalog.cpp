#include <climits>
#include "ProductCatalog.hpp"
#include "FileUtils.hpp"
#include "MenuUtils.hpp"
#include "InputUtils.hpp"
#include "User.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

// ── Helpers ──────────────────────────────────
static string fmtPrice(double v) {
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return oss.str();
}

// Returns the lowest positive integer NOT already used as an ID
int ProductCatalog::getNextId() const {
    int candidate = 1;
    while (true) {
        bool used = false;
        for (auto& p : products)
            if (p.getId() == candidate) { used = true; break; }
        if (!used)
            for (auto& p : pending)
                if (p.getId() == candidate) { used = true; break; }
        if (!used) return candidate;
        ++candidate;
    }
}

ProductCatalog::ProductCatalog(string dataFile, string pendingFile)
    : dataFile(dataFile), pendingFile(pendingFile) {
    products = loadProducts(dataFile);
    pending  = loadPending(pendingFile);
}

ProductCatalog::~ProductCatalog() {
    saveProducts(dataFile,    products);
    savePending (pendingFile, pending);
}

// ── Low stock check (returns count, used for login alert) ────
int ProductCatalog::countLowStock() const {
    int count = 0;
    for (auto& p : products)
        if (p.getStock() < 3) ++count;
    return count;
}

// ── Paginated view ────────────────────────────
void ProductCatalog::viewPaginated(const vector<Product>& list, const string& title) {
    if (list.empty()) {
        cout << "No products found.\n";
        return;
    }
    const int PAGE = 10;
    int total = (int)list.size();
    int totalPages = (total + PAGE - 1) / PAGE;
    int page = 0;

    while (true) {
        clearScreen();
        cout << "  [ " << title << " ]  Page " << (page + 1) << " / " << totalPages
             << "  (total: " << total << ")\n\n";

        int start = page * PAGE;
        int end   = min(start + PAGE, total);
        vector<Product> slice(list.begin() + start, list.begin() + end);
        printProducts(slice);

        cout << "  [N] Next   [P] Prev   [Q] Quit\n  >> ";
        char c; cin >> c; c = tolower(c);
        if      (c == 'n' && page < totalPages - 1) ++page;
        else if (c == 'p' && page > 0)              --page;
        else if (c == 'q')                           break;
    }
}

// ── Add (admin direct) ───────────────────────
void ProductCatalog::addProduct(const string& username) {
    cout << "\n[ ADD PRODUCT ]\n";
    flushLine();
    string name     = getNonEmptyString("Name     : ");
    string category = getNonEmptyString("Category : ");
    double price    = getPositiveDouble ("Price  $ : ");
    int    stock    = getInt            ("Stock    : ", 0, INT_MAX);
    int    id       = getNextId();
    products.emplace_back(id, name, category, price, stock, username);
    saveProducts(dataFile, products);
    cout << "  Product added successfully (ID: " << id << ").\n";
}

// ── Submit (user → pending) ──────────────────
void ProductCatalog::submitProduct(const string& username) {
    cout << "\n[ SUBMIT PRODUCT FOR APPROVAL ]\n";
    flushLine();
    string name     = getNonEmptyString("Name     : ");
    string category = getNonEmptyString("Category : ");
    double price    = getPositiveDouble ("Price  $ : ");
    int    stock    = getInt            ("Stock    : ", 0, INT_MAX);
    int    id       = getNextId();
    pending.emplace_back(id, name, category, price, stock, username);
    savePending(pendingFile, pending);
    cout << "Submitted! Waiting for admin approval.\n";
}

// ── View my own products (user) ───────────────
void ProductCatalog::viewMyProducts(const string& username) {
    cout << "\n[ MY PRODUCTS ]\n";
    vector<Product> mine;
    for (auto& p : products)
        if (p.getOwner() == username) mine.push_back(p);
    if (mine.empty()) {
        cout << "You have no approved products yet.\n";
        return;
    }
    viewPaginated(mine, "MY PRODUCTS - " + username);
}

// ── Request update (user edits → goes to pending) ────────────
void ProductCatalog::requestUpdateProduct(const string& username) {
    cout << "\n[ REQUEST PRODUCT UPDATE ]\n";

    // Only show products owned by this user
    vector<Product> mine;
    for (auto& p : products)
        if (p.getOwner() == username) mine.push_back(p);

    if (mine.empty()) {
        cout << "You have no approved products to edit.\n";
        return;
    }
    printProducts(mine);

    flushLine();
    string idStr = getOptionalString("Enter product ID to edit (or press Enter to cancel): ");
    if (idStr.empty()) { cout << "Cancelled.\n"; return; }

    int id;
    try { id = stoi(idStr); } catch (...) { cout << "  Invalid ID.\n"; return; }

    for (auto& p : products) {
        if (p.getId() == id && p.getOwner() == username) {
            cout << "  (Leave blank to keep current value)\n";
            string name     = getOptionalString("New Name     [" + p.getName()             + "]: ");
            string category = getOptionalString("New Category [" + p.getCategory()         + "]: ");
            string priceStr = getOptionalString("New Price  $ [" + fmtPrice(p.getPrice())  + "]: ");
            string stockStr = getOptionalString("New Stock    [" + to_string(p.getStock()) + "]: ");

            
            string newName     = name.empty()     ? p.getName()     : name;
            string newCategory = category.empty() ? p.getCategory() : category;
            double newPrice    = p.getPrice();
            int    newStock    = p.getStock();

            if (!priceStr.empty()) {
                try {
                    double v = stod(priceStr);
                    if (v > 0) newPrice = v;
                    else cout << "Price must be > 0. Keeping current.\n";
                } catch (...) { cout << "Invalid price. Keeping current.\n"; }
            }
            if (!stockStr.empty()) {
                try {
                    int v = stoi(stockStr);
                    if (v >= 0) newStock = v;
                    else cout << "Stock cannot be negative. Keeping current.\n";
                } catch (...) { cout << "Invalid stock. Keeping current.\n"; }
            }

            // Put the edited version in pending with the SAME id so admin
            // knows which product it is, but don't touch products[] yet.
            // We reuse the existing ID so approval can replace/update it.
            pending.emplace_back(id, newName, newCategory, newPrice, newStock, username);
            savePending(pendingFile, pending);
            cout << "Edit submitted for admin approval.\n";
            cout << "Your current product stays unchanged until the admin approves.\n";
            return;
        }
    }
    cout << "Product ID not found or not owned by you.\n";
}

// ── Take out product (user decreases stock) ──
void ProductCatalog::takeOutProduct(const string& username) {
    cout << "\n[ TAKE OUT PRODUCT ]\n";

    vector<Product> mine;
    for (auto& p : products)
        if (p.getOwner() == username) mine.push_back(p);

    if (mine.empty()) {
        cout << "You have no approved products.\n";
        return;
    }
    printProducts(mine);

    int id  = getInt("Enter product ID to take out from: ", 1, INT_MAX);
    int qty = getInt("Quantity to take out: ", 1, INT_MAX);

    for (auto& p : products) {
        if (p.getId() == id && p.getOwner() == username) {
            if (qty > p.getStock()) {
                cout << "Not enough stock (available: " << p.getStock() << ").\n";
            } else {
                p.setStock(p.getStock() - qty);
                saveProducts(dataFile, products);
                cout << "Done. New stock for \"" << p.getName()
                     << "\": " << p.getStock() << "\n";
            }
            return;
        }
    }
    cout << "Product ID not found or not owned by you.\n";
}

// ── Delete all products owned by a user ──────
void ProductCatalog::deleteByOwner(const string& username) {
    products.erase(
        remove_if(products.begin(), products.end(),
            [&](const Product& p){ return p.getOwner() == username; }),
        products.end()
    );
    pending.erase(
        remove_if(pending.begin(), pending.end(),
            [&](const Product& p){ return p.getOwner() == username; }),
        pending.end()
    );
    saveProducts(dataFile,    products);
    savePending (pendingFile, pending);
    cout << "All products owned by \"" << username << "\" deleted.\n";
}

// ── Low stock list (admin option 8) ──────────
void ProductCatalog::checkLowStock() {
    cout << "\n[ LOW STOCK PRODUCTS (stock < 3) ]\n";
    vector<Product> low;
    for (auto& p : products)
        if (p.getStock() < 3) low.push_back(p);
    if (low.empty())
        cout << "All products have sufficient stock.\n";
    else
        viewPaginated(low, "LOW STOCK PRODUCTS");
}

// ── View all user accounts (admin) ───────────
void ProductCatalog::viewAllUsers(const vector<User*>& users) {
    cout << "\n[ ALL USER ACCOUNTS ]\n\n";
    cout << "  " << left << setw(20) << "Username"
         << setw(12) << "Role" << "\n";
    cout << "  " << string(32, '-') << "\n";
    for (auto* u : users) {
        cout << "  " << setw(20) << u->getUsername()
             << setw(12) << u->getRole() << "\n";
    }
    cout << "\n";
}

// ── Review pending (admin) ───────────────────
void ProductCatalog::reviewPending() {
    cout << "\n[ REVIEW PENDING PRODUCTS ]\n";
    if (pending.empty()) {
        cout << "No pending products to review.\n";
        return;
    }
    vector<Product> stillPending;
    for (auto& p : pending) {
        cout << "\n";
        printProducts({p});
        cout << "Approve this product? (y = approve / n = reject / s = skip): ";
        char c; cin >> c;
        if (c == 'y' || c == 'Y') {
            bool replaced = false;
            for (auto& existing : products) {
                if (existing.getId() == p.getId()) {
                    existing = p;
                    replaced = true;
                    break;
                }
            }
            if (!replaced) products.push_back(p);
            saveProducts(dataFile, products);
            cout << (replaced ? "Edit approved! Product updated.\n" : "  Approved!\n");
        } else if (c == 'n' || c == 'N') {
            cout << "  Rejected.\n";
        } else {
            stillPending.push_back(p);
            cout << "  Skipped.\n";
        }
    }
    pending = stillPending;
    savePending(pendingFile, pending);
}

// ── View by Category ─────────────────────────
void ProductCatalog::viewByCategory() {
    cout << "\n[ VIEW PRODUCTS ]\n";
    flushLine();
    string cat = getOptionalString("Category (leave blank for ALL): ");
    if (cat.empty()) {
        viewPaginated(products, "ALL PRODUCTS");
        return;
    }
    vector<Product> filtered;
    for (auto& p : products)
        if (p.getCategory() == cat) filtered.push_back(p);
    viewPaginated(filtered, "CATEGORY: " + cat);
}

// ── Update ───────────────────────────────────
void ProductCatalog::updateProduct() {
    cout << "\n[ UPDATE PRODUCT ]\n";
    int id = getInt("Enter product ID to update: ", 1, INT_MAX);
    flushLine();
    for (auto& p : products) {
        if (p.getId() == id) {
            cout << "(Leave blank to keep current value)\n";
            string name     = getOptionalString("New Name     [" + p.getName()             + "]: ");
            string category = getOptionalString("New Category [" + p.getCategory()         + "]: ");
            string priceStr = getOptionalString("New Price  $ [" + fmtPrice(p.getPrice())  + "]: ");
            string stockStr = getOptionalString("New Stock    [" + to_string(p.getStock()) + "]: ");
            if (!name.empty())     p.setName(name);
            if (!category.empty()) p.setCategory(category);
            if (!priceStr.empty()) {
                try {
                    double v = stod(priceStr);
                    if (v > 0) p.setPrice(v);
                    else cout << "Price must be > 0. Keeping current.\n";
                } catch (...) { cout << "Invalid price. Keeping current.\n"; }
            }
            if (!stockStr.empty()) {
                try {
                    int v = stoi(stockStr);
                    if (v >= 0) p.setStock(v);
                    else cout << "Stock cannot be negative. Keeping current.\n";
                } catch (...) { cout << "Invalid stock. Keeping current.\n"; }
            }
            saveProducts(dataFile, products);
            cout << "Product updated.\n";
            return;
        }
    }
    cout << "Product ID not found.\n";
}

// ── Delete ───────────────────────────────────
void ProductCatalog::deleteProduct() {
    cout << "\n[ DELETE PRODUCT ]\n";
    
    flushLine();
    string idStr = getOptionalString("Enter product ID to delete (or press Enter to cancel): ");
    if (idStr.empty()) {
        cout << "  Cancelled.\n";
        return;
    }
    int id;
    try { id = stoi(idStr); } catch (...) { cout << "Invalid ID.\n"; return; }

    for (auto it = products.begin(); it != products.end(); it++) {
        if (it->getId() == id) {
            cout << "About to delete: " << it->getName()
                 << " (Owner: " << it->getOwner() << ")\n";
            cout << "Are you sure? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                products.erase(it);
                saveProducts(dataFile, products);
                cout << "Product deleted.\n";
            } else {
                cout << "Cancelled.\n";
            }
            return;
        }
    }
    cout << "Product ID not found.\n";
}

// ── Search ────────────────────────────────────
void ProductCatalog::searchProducts() {
    cout << "\n[ SEARCH PRODUCTS ]\n";
    int choice = getInt("Search by  (1) Name  (2) ID  (3) Category: ", 1, 3);
    flushLine();
    vector<Product> results;
    if (choice == 1) {
        string kw = getNonEmptyString("Keyword: ");
        string kwLower = kw;
        for (char& c : kwLower) c = tolower(c);
        for (auto& p : products) {
            string n = p.getName();
            for (char& c : n) c = tolower(c);
            if (n.find(kwLower) != string::npos) results.push_back(p);
        }
    } else if (choice == 2) {
        int id = getInt("Product ID: ", 1, INT_MAX);
        for (auto& p : products)
            if (p.getId() == id) results.push_back(p);
    } else {
        string cat = getNonEmptyString("Category: ");
        for (auto& p : products)
            if (p.getCategory() == cat) results.push_back(p);
    }
    viewPaginated(results, "SEARCH RESULTS");
}

// ── Sort ──────────────────────────────────────
void ProductCatalog::sortProducts() {
    cout << "\n[ SORT PRODUCTS ]\n";
    int by    = getInt("Sort by  (1) Price  (2) Stock: ", 1, 2);
    int order = getInt("Order    (1) Ascending  (2) Descending: ", 1, 2);
    vector<Product> sorted = products;
    if (by == 1) {
        sort(sorted.begin(), sorted.end(), [&](const Product& a, const Product& b) {
            return order == 1 ? a.getPrice() < b.getPrice() : a.getPrice() > b.getPrice();
        });
    } else {
        sort(sorted.begin(), sorted.end(), [&](const Product& a, const Product& b) {
            return order == 1 ? a.getStock() < b.getStock() : a.getStock() > b.getStock();
        });
    }
    viewPaginated(sorted, "SORTED PRODUCTS");
}