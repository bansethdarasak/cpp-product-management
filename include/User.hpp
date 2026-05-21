#pragma once
#include <string>

using std::string;


class User {
protected:
    string username;
    string password;
    bool   adminFlag;

public:
    User(string username, string password, bool adminFlag);
    virtual ~User() {}

    string getUsername() const;
    string getPassword() const;
    bool   isAdmin()     const;

    virtual string getRole() const = 0;   
};

class AdminUser : public User {
public:
    AdminUser(string username, string password);
    string getRole() const override;
};

class RegularUser : public User {
public:
    RegularUser(string username, string password);
    string getRole() const override;
};
