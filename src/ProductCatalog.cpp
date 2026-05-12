#include <climits>
#include "ProductCatalog.hpp"
#include "FileUtils.hpp"
#include "MenuUtils.hpp"
#include "InputUtils.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

ProductCatalog::ProductCatalog(string dataFile, string pendingFile)
    : dataFile(dataFile), pendingFile(pendingFile), nextId(1) {
    products = loadProducts(dataFile);
    pending  = loadPending(pendingFile);

    // find highest ID across both lists so IDs never clash
    for (auto& p : products)
        if (p.getId() >= nextId) nextId = p.getId() + 1;
    for (auto& p : pending)
        if (p.getId() >= nextId) nextId = p.getId() + 1;
}

ProductCatalog::~ProductCatalog() {
    saveProducts(dataFile,    products);
    savePending (pendingFile, pending);
}

// ── Add (admin direct) ───────────────────────
void ProductCatalog::addProduct() {
    cout << "\n[ ADD PRODUCT ]\n";
    flushLine();

    string name     = getNonEmptyString("Name     : ");
    string category = getNonEmptyString("Category : ");
    double price    = getPositiveDouble ("Price  $ : ");
    int    stock    = getInt            ("Stock    : ", 0, INT_MAX);

    products.emplace_back(nextId++, name, category, price, stock);
    saveProducts(dataFile, products);
    cout << "  Product added successfully.\n";
}

// ── Submit (user — goes to pending) ──────────
void ProductCatalog::submitProduct() {
    cout << "\n[ SUBMIT PRODUCT FOR APPROVAL ]\n";
    flushLine();

    string name     = getNonEmptyString("Name     : ");
    string category = getNonEmptyString("Category : ");
    double price    = getPositiveDouble ("Price  $ : ");
    int    stock    = getInt            ("Stock    : ", 0, INT_MAX);

    pending.emplace_back(nextId++, name, category, price, stock);
    savePending(pendingFile, pending);
    cout << "  Submitted! Waiting for admin approval.\n";
}

// ── Review pending (admin) ───────────────────
void ProductCatalog::reviewPending() {
    cout << "\n[ REVIEW PENDING PRODUCTS ]\n";

    if (pending.empty()) {
        cout << "  No pending products to review.\n";
        return;
    }

    // walk through each pending product one by one
    vector<Product> stillPending;

    for (auto& p : pending) {
        cout << "\n";
        printProducts({p});
        cout << "  Approve this product? (y = approve / n = reject / s = skip): ";
        char c; cin >> c;

        if (c == 'y' || c == 'Y') {
            products.push_back(p);
            saveProducts(dataFile, products);
            cout << "  Approved! Product added to catalog.\n";
        } else if (c == 'n' || c == 'N') {
            cout << "  Rejected. Product removed.\n";
            // just don't add to stillPending — it's gone
        } else {
            // skip — leave it in pending for later
            stillPending.push_back(p);
            cout << "  Skipped. Will review later.\n";
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
        printProducts(products);
        return;
    }

    vector<Product> filtered;
    for (auto& p : products)
        if (p.getCategory() == cat) filtered.push_back(p);
    printProducts(filtered);
}

// ── Update ────────────────────────────────────
void ProductCatalog::updateProduct() {
    cout << "\n[ UPDATE PRODUCT ]\n";
    int id = getInt("Enter product ID to update: ", 1, INT_MAX);
    flushLine();

    for (auto& p : products) {
        if (p.getId() == id) {
            cout << "  (Leave blank to keep current value)\n";

            string name     = getOptionalString("New Name     [" + p.getName()     + "]: ");
            string category = getOptionalString("New Category [" + p.getCategory() + "]: ");
            string priceStr = getOptionalString("New Price  $ [" + to_string(p.getPrice()) + "]: ");
            string stockStr = getOptionalString("New Stock    [" + to_string(p.getStock()) + "]: ");

            if (!name.empty())     p.setName(name);
            if (!category.empty()) p.setCategory(category);

            if (!priceStr.empty()) {
                try {
                    double v = stod(priceStr);
                    if (v > 0) p.setPrice(v);
                    else cout << "  Price must be > 0. Keeping current.\n";
                } catch (...) {
                    cout << "  Invalid price. Keeping current.\n";
                }
            }

            if (!stockStr.empty()) {
                try {
                    int v = stoi(stockStr);
                    if (v >= 0) p.setStock(v);
                    else cout << "  Stock cannot be negative. Keeping current.\n";
                } catch (...) {
                    cout << "  Invalid stock. Keeping current.\n";
                }
            }

            saveProducts(dataFile, products);
            cout << "  Product updated.\n";
            return;
        }
    }
    cout << "  Product ID not found.\n";
}

// ── Delete ────────────────────────────────────
void ProductCatalog::deleteProduct() {
    cout << "\n[ DELETE PRODUCT ]\n";
    int id = getInt("Enter product ID to delete: ", 1, INT_MAX);

    for (auto it = products.begin(); it != products.end(); it++) {
        if (it->getId() == id) {
            cout << "  About to delete: " << it->getName() << "\n";
            cout << "  Are you sure? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                products.erase(it);
                saveProducts(dataFile, products);
                cout << "  Product deleted.\n";
            } else {
                cout << "  Cancelled.\n";
            }
            return;
        }
    }
    cout << "  Product ID not found.\n";
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
            if (n.find(kwLower) != string::npos)
                results.push_back(p);
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

    printProducts(results);
}

// ── Sort ──────────────────────────────────────
void ProductCatalog::sortProducts() {
    cout << "\n[ SORT PRODUCTS ]\n";
    int by    = getInt("Sort by  (1) Price  (2) Stock: ", 1, 2);
    int order = getInt("Order    (1) Ascending  (2) Descending: ", 1, 2);

    vector<Product> sorted = products;

    if (by == 1) {
        sort(sorted.begin(), sorted.end(), [&](const Product& a, const Product& b) {
            return order == 1 ? a.getPrice() < b.getPrice()
                              : a.getPrice() > b.getPrice();
        });
    } else {
        sort(sorted.begin(), sorted.end(), [&](const Product& a, const Product& b) {
            return order == 1 ? a.getStock() < b.getStock()
                              : a.getStock() > b.getStock();
        });
    }

    printProducts(sorted);
}