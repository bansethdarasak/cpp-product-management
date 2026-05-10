#include "AuthManager.hpp"
#include "FileUtils.hpp"
#include "InputUtils.hpp"
#include <iostream>

using namespace std;

AuthManager::AuthManager(string accountFile) : accountFile(accountFile) {
    users = loadAccounts(accountFile);

    // Seed a default admin if no accounts exist
    bool hasAdmin = false;
    for (auto* u : users)
        if (u->isAdmin()) { hasAdmin = true; break; }

    if (!hasAdmin) {
        users.push_back(new AdminUser("admin", "admin123"));
        saveAccounts(accountFile, users);
    }
}

AuthManager::~AuthManager() {
    for (auto* u : users) delete u;
}

bool AuthManager::usernameExists(const string& username) {
    for (auto* u : users)
        if (u->getUsername() == username) return true;
    return false;
}

// Returns a raw pointer into the internal list — caller does NOT delete it
User* AuthManager::login() {
    cout << "\n[ LOGIN ]\n";
    string uname = getUsername("Username: ");
    string pass  = getPassword("Password: ");

    for (auto* u : users)
        if (u->getUsername() == uname && u->getPassword() == pass)
            return u;

    cout << "  Invalid credentials.\n";
    return nullptr;
}

bool AuthManager::signup() {
    cout << "\n[ SIGN UP ]\n";
    string uname = getUsername("Username (3-20 chars, no spaces): ");

    if (usernameExists(uname)) {
        cout << "  Username already taken.\n";
        return false;
    }

    string pass = getPassword("Password (min 6 chars): ");
    users.push_back(new RegularUser(uname, pass));
    saveAccounts(accountFile, users);
    cout << "  Account created! You can now log in.\n";
    return true;
}
