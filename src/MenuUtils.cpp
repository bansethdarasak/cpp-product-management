#include "MenuUtils.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tabulate/table.hpp>

using namespace std;
using namespace tabulate;

// ANSI escape codes — true orange (256-color code 214)
static const string ORANGE = "\033[38;5;214m";
static const string RESET  = "\033[0m";

static string fmtPrice(double v) {
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return oss.str();
}

// Prints table lines, wrapping every line with orange color
static void printCentered(Table& t, int padding = 10) {
    string indent(padding, ' ');
    stringstream ss;
    ss << t;
    string line;
    while (getline(ss, line))
        cout << ORANGE << indent << line << RESET << "\n";
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

// Standard product table — shows Owner column so ownership is visible
void printProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "No products found.\n";
        return;
    }
    Table t;
    t.add_row({"ID", "Name", "Category", "Price ($)", "Stock", "Owner"});
    for (auto& p : products) {
        string stockStr = to_string(p.getStock());
        if (p.getStock() < 3) stockStr += " !!";
        t.add_row({
            to_string(p.getId()),
            p.getName(),
            p.getCategory(),
            fmtPrice(p.getPrice()),
            stockStr,
            p.getOwner()
        });
    }

    t[0].format().font_style({FontStyle::bold});
    printCentered(t, 4);
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