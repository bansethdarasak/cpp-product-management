#include "MenuUtils.hpp"
#include <iostream>
#include <tabulate/table.hpp>

using namespace std;
using namespace tabulate;

void printMenu(const vector<string>& items) {
    Table t;
    t.add_row({"No", "Option"});
    for (size_t i = 0; i < items.size(); i++)
        t.add_row({to_string(i + 1), items[i]});
    t[0].format().font_style({FontStyle::bold});
    cout << t << endl;
}

void printProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "  No products found.\n";
        return;
    }

    Table t;
    t.add_row({"ID", "Name", "Category", "Price ($)", "Stock"});
    for (auto& p : products) {
        t.add_row({
            to_string(p.getId()),
            p.getName(),
            p.getCategory(),
            to_string(p.getPrice()),
            to_string(p.getStock())
        });
    }
    t[0].format().font_style({FontStyle::bold});
    cout << t << endl;
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
