#pragma once
#include <string>
#include <vector>
#include "User.hpp"
#include "ProductCatalog.hpp"

using std::string;
using std::vector;

class AuthManager {
private:
    vector<User*> users;
    string        accountFile;

    bool usernameExists(const string& username);

public:
    AuthManager(string accountFile);
    ~AuthManager();

    User* login();
    bool  signup();
    void  deleteUser(ProductCatalog& catalog);
};