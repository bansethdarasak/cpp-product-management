#include <iostream>
#include <vector>
#include <string>
#include "AuthManager.hpp"
#include "ProductCatalog.hpp"
#include "MenuUtils.hpp"
#include "InputUtils.hpp"

using namespace std;

void deleteMenu(ProductCatalog& catalog, AuthManager& auth) {
    cout << "\n[ DELETE ]\n";
    cout << "  1. Delete User Account\n";
    cout << "  2. Delete Product\n";
    int choice = getInt(">> Choose option: ", 1, 2);
    if (choice == 1)
        auth.deleteUser(catalog);
    else
        catalog.deleteProduct();
}

void productMenu(User* user, ProductCatalog& catalog, AuthManager& auth) {

    vector<string> adminMenu = {
        "View Products (by Category)",
        "Search Products",
        "Sort Products",
        "Add Product",
        "Update Product",
        "Delete Account / Product",
        "Review Pending Products",
        "Logout"
    };

    vector<string> userMenu = {
        "View Products (by Category)",
        "Search Products",
        "Sort Products",
        "Submit Product for Approval",
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

        int maxOption = user->isAdmin() ? 8 : 5;
        int choice = getInt(">> Choose option: ", 1, maxOption);

        if (user->isAdmin()) {
            switch (choice) {
                case 1: catalog.viewByCategory();                    break;
                case 2: catalog.searchProducts();                    break;
                case 3: catalog.sortProducts();                      break;
                case 4: catalog.addProduct(user->getUsername());     break;
                case 5: catalog.updateProduct();                     break;
                case 6: deleteMenu(catalog, auth);                   break;
                case 7: catalog.reviewPending();                     break;
                case 8: running = false; cout << "  Logged out.\n"; break;
            }
        } else {
            switch (choice) {
                case 1: catalog.viewByCategory();                     break;
                case 2: catalog.searchProducts();                     break;
                case 3: catalog.sortProducts();                       break;
                case 4: catalog.submitProduct(user->getUsername());   break;
                case 5: running = false; cout << "  Logged out.\n";  break;
            }
        }

        if (running) pauseScreen();
    }
}

int main() {
    AuthManager    auth("accounts.xlsx");
    ProductCatalog catalog("products.xlsx", "pending.xlsx");

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
                if (user) productMenu(user, catalog, auth);
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