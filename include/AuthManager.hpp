#pragma once
#include <string>
#include <vector>
#include "User.hpp"

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

    // Returns nullptr if login fails (caller does NOT own the pointer)
    User* login();
    bool  signup();
};
