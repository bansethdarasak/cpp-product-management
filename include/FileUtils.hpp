#pragma once
#include <string>
#include <vector>
#include "Product.hpp"
#include "User.hpp"

using std::string;
using std::vector;

void            saveProducts(const string& filename, const vector<Product>& products);
vector<Product> loadProducts(const string& filename);

// pending products — same structure, separate file
void            savePending(const string& filename, const vector<Product>& products);
vector<Product> loadPending(const string& filename);

void          saveAccounts(const string& filename, const vector<User*>& users);
vector<User*> loadAccounts(const string& filename);