#include <iostream>
#include <vector>
#include <string>
#include "AuthManager.hpp"
#include "ProductCatalog.hpp"
#include "MenuUtils.hpp"
#include "InputUtils.hpp"

using namespace std;

void productMenu(User* user, ProductCatalog& catalog) {
    vector<string> adminMenu = {
        "View Products (by Category)",
        "Search Products",
        "Sort Products",
        "Add Product",
        "Update Product",
        "Delete Product",
        "Logout"
    };
    vector<string> userMenu = {
        "View Products (by Category)",
        "Search Products",
        "Sort Products",
        "Logout"
    };

    bool running = true;
    while (running) {
        clearScreen();
        cout << "  Logged in as: " << user->getUsername()
             << " [" << user->getRole() << "]\n\n";

        if (user->isAdmin())
            printMenu(adminMenu);
        else
            printMenu(userMenu);

        int maxOption = user->isAdmin() ? 7 : 4;
        int choice = getInt(">> Choose option: ", 1, maxOption);

        if (user->isAdmin()) {
            switch (choice) {
                case 1: catalog.viewByCategory(); break;
                case 2: catalog.searchProducts(); break;
                case 3: catalog.sortProducts();   break;
                case 4: catalog.addProduct();     break;
                case 5: catalog.updateProduct();  break;
                case 6: catalog.deleteProduct();  break;
                case 7: running = false; cout << "  Logged out.\n"; break;
            }
        } else {
            switch (choice) {
                case 1: catalog.viewByCategory(); break;
                case 2: catalog.searchProducts(); break;
                case 3: catalog.sortProducts();   break;
                case 4: running = false; cout << "  Logged out.\n"; break;
            }
        }

        if (running) pauseScreen();
    }
}

int main() {
    AuthManager   auth("accounts.xlsx");
    ProductCatalog catalog("products.xlsx");

    vector<string> mainMenu = {
        "Login",
        "Sign Up",
        "Exit"
    };

    bool running = true;
    while (running) {
        clearScreen();
        cout << "\n  === PRODUCT MANAGEMENT SYSTEM ===\n\n";
        printMenu(mainMenu);

        int choice = getInt(">> Choose option: ", 1, 3);

        switch (choice) {
            case 1: {
                User* user = auth.login();
                if (user) productMenu(user, catalog);
                else pauseScreen();
                break;
            }
            case 2:
                auth.signup();
                pauseScreen();
                break;
            case 3:
                running = false;
                cout << "\n  Goodbye!\n\n";
                break;
        }
    }

    return 0;
}
