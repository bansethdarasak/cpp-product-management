#pragma once
#include <string>
#include <vector>
#include "Product.hpp"
#include "User.hpp"

using std::string;
using std::vector;

void            saveProducts(const string& filename, const vector<Product>& products);
vector<Product> loadProducts(const string& filename);

void          saveAccounts(const string& filename, const vector<User*>& users);
vector<User*> loadAccounts(const string& filename);
