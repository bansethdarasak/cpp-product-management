#include "MenuUtils.hpp"
#include <iostream>
#include <sstream>
#include <tabulate/table.hpp>
#include <iomanip>

using namespace std;
using namespace tabulate;

static void printCentered(Table& t, int padding = 10) {
    string indent(padding, ' ');
    stringstream ss;
    ss << t;
    string line;
    while (getline(ss, line))
        cout << indent << line << "\n";
    cout << "\n";
}

void printMenu(const vector<string>& items) {
    Table t;
    t.add_row({"No", "Option"});
    for (size_t i = 0; i < items.size(); i++)
        t.add_row({to_string(i + 1), items[i]});
    t[0].format().font_style({FontStyle::bold});
    printCentered(t);
}

void printProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "  No products found.\n";
        return;
    }
    Table t;
    t.add_row({"ID", "Name", "Category", "Price ($)", "Stock", "Ownership"});
    for (auto& p : products) {
        t.add_row({
            to_string(p.getId()),
            p.getName(),
            p.getCategory(),
            ([&]{ ostringstream oss; oss << fixed << setprecision(2) << p.getPrice(); return oss.str(); })(),
            to_string(p.getStock()),
            p.getOwner()
        });
    }
    t[0].format().font_style({FontStyle::bold});
    printCentered(t, 6);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}