#include "FileUtils.hpp"
#include <iostream>
#include <xlnt/xlnt.hpp>

using namespace std;

// ── helper: write products ───────────────────
static void writeProductSheet(xlnt::worksheet& ws, const vector<Product>& products) {
    ws.cell("A1").value("ID");
    ws.cell("B1").value("Name");
    ws.cell("C1").value("Category");
    ws.cell("D1").value("Price");
    ws.cell("E1").value("Stock");
    ws.cell("F1").value("Owner");

    int row = 2;
    for (auto& p : products) {
        ws.cell("A" + to_string(row)).value(p.getId());
        ws.cell("B" + to_string(row)).value(p.getName());
        ws.cell("C" + to_string(row)).value(p.getCategory());
        ws.cell("D" + to_string(row)).value(p.getPrice());
        ws.cell("E" + to_string(row)).value(p.getStock());
        ws.cell("F" + to_string(row)).value(p.getOwner());
        row++;
    }
}

// ── helper: read products ────────────────────
static vector<Product> readProductSheet(const string& filename) {
    vector<Product> products;
    xlnt::workbook wb;
    try { wb.load(filename); } catch (...) { return products; }
    auto ws = wb.active_sheet();
    for (auto row : ws.rows(false)) {
        if (row[0].to_string() == "ID") continue;
        try {
            int    id       = stoi(row[0].to_string());
            string name     = row[1].to_string();
            string category = row[2].to_string();
            double price    = stod(row[3].to_string());
            int    stock    = stoi(row[4].to_string());
            string owner    = "admin";
            try { owner = row[5].to_string(); } catch (...) {}
            if (owner.empty()) owner = "admin";
            products.emplace_back(id, name, category, price, stock, owner);
        } catch (...) {}
    }
    return products;
}

// ── Products ─────────────────────────────────
void saveProducts(const string& filename, const vector<Product>& products) {
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Products");
    writeProductSheet(ws, products);
    wb.save(filename);
    cout << "Data saved to " << filename << "\n";
}

vector<Product> loadProducts(const string& filename) {
    return readProductSheet(filename);
}

// ── Pending ───────────────────────────────────
void savePending(const string& filename, const vector<Product>& products) {
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Pending");
    writeProductSheet(ws, products);
    wb.save(filename);
}

vector<Product> loadPending(const string& filename) {
    return readProductSheet(filename);
}

// ── Accounts ─────────────────────────────────
void saveAccounts(const string& filename, const vector<User*>& users) {
    xlnt::workbook wb;
    auto ws = wb.active_sheet();
    ws.title("Accounts");
    ws.cell("A1").value("Username");
    ws.cell("B1").value("Password");
    ws.cell("C1").value("IsAdmin");
    int row = 2;
    for (auto* u : users) {
        ws.cell("A" + to_string(row)).value(u->getUsername());
        ws.cell("B" + to_string(row)).value(u->getPassword());
        ws.cell("C" + to_string(row)).value(u->isAdmin() ? 1 : 0);
        row++;
    }
    wb.save(filename);
}

vector<User*> loadAccounts(const string& filename) {
    vector<User*> users;
    xlnt::workbook wb;
    try { wb.load(filename); } catch (...) { return users; }
    auto ws = wb.active_sheet();
    for (auto row : ws.rows(false)) {
        if (row[0].to_string() == "Username") continue;
        try {
            string uname   = row[0].to_string();
            string pass    = row[1].to_string();
            bool   isAdmin = (row[2].to_string() == "1");
            if (isAdmin) users.push_back(new AdminUser(uname, pass));
            else         users.push_back(new RegularUser(uname, pass));
        } catch (...) {}
    }
    return users;
}