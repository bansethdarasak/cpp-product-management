#include "User.hpp"

// ── User ─────────────────────────────────────
User::User(string username, string password, bool adminFlag)
    : username(username), password(password), adminFlag(adminFlag) {}

string User::getUsername() const { return username; }
string User::getPassword() const { return password; }
bool   User::isAdmin()     const { return adminFlag; }

// ── AdminUser ────────────────────────────────
AdminUser::AdminUser(string username, string password)
    : User(username, password, true) {}

string AdminUser::getRole() const { return "Admin"; }

// ── RegularUser ──────────────────────────────
RegularUser::RegularUser(string username, string password)
    : User(username, password, false) {}

string RegularUser::getRole() const { return "User"; }
