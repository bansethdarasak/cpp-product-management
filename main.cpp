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
        "Low Stock Products",
        "View All User Accounts",
        "Logout"
    };

    vector<string> userMenu = {
        "View Products (by Category)",
        "My Products",                          
        "Search Products",
        "Sort Products",
        "Submit Product for Approval",
        "Request Product Update",               
        "Take Out Product (decrease stock)",
        "Logout"
    };

    // ── Low stock alert shown once right after admin logs in ──
    if (user->isAdmin()) {
        int lowCount = catalog.countLowStock();
        if (lowCount > 0) {
            cout << "\n*** LOW STOCK ALERT: " << lowCount << " product(s) have stock below 3! \n";
            cout << "\nPress Enter to continue...";
            cin.ignore(); cin.get();
        }
    }

    bool running = true;
    while (running) {
        clearScreen();
        cout << "  Logged in as: " << user->getUsername()
             << " [" << user->getRole() << "]\n";

        // ── Persistent low-stock reminder in the menu header ──
        if (user->isAdmin()) {
            int lowCount = catalog.countLowStock();
            if (lowCount > 0)
                cout << "\n*** LOW STOCK on " << lowCount << " product(s)! Check option 8 ***\n";
        }
        cout << "\n";

        if (user->isAdmin())
            printMenu(adminMenu);
        else
            printMenu(userMenu);

        int maxOption = user->isAdmin() ? 10 : 8;
        int choice = getInt(">> Choose option: ", 1, maxOption);

        if (user->isAdmin()) {
            switch (choice) {
                case 1:  catalog.viewByCategory();                    break;
                case 2:  catalog.searchProducts();                    break;
                case 3:  catalog.sortProducts();                      break;
                case 4:  catalog.addProduct(user->getUsername());     break;
                case 5:  catalog.updateProduct();                     break;
                case 6:  deleteMenu(catalog, auth);                   break;
                case 7:  catalog.reviewPending();                     break;
                case 8:  catalog.checkLowStock();                     break;
                case 9:  catalog.viewAllUsers(auth.getUsers());       break;
                case 10: running = false; cout << "  Logged out.\n"; break;
            }
        } else {
            switch (choice) {
                case 1: catalog.viewByCategory();                           break;
                case 2: catalog.viewMyProducts(user->getUsername());        break;
                case 3: catalog.searchProducts();                           break;
                case 4: catalog.sortProducts();                             break;
                case 5: catalog.submitProduct(user->getUsername());         break;
                case 6: catalog.requestUpdateProduct(user->getUsername());  break;
                case 7: catalog.takeOutProduct(user->getUsername());        break;
                case 8: running = false; cout << "  Logged out.\n";        break;
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
                cout << "\nThanks you ! Have a wonderful day !\n\n";
                break;
        }
    }

    return 0;
}