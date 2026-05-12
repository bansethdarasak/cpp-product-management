#pragma once
#include <string>
#include <vector>
#include "Product.hpp"

using std::string;
using std::vector;

class ProductCatalog {
private:
    vector<Product> products;
    vector<Product> pending;
    int             nextId;
    string          dataFile;
    string          pendingFile;

public:
    ProductCatalog(string dataFile, string pendingFile);
    ~ProductCatalog();

    // Admin only
    void addProduct(const string& username);
    void updateProduct();
    void deleteProduct();
    void reviewPending();
    void deleteByOwner(const string& username);  // wipe all products when user deleted

    // User only
    void submitProduct(const string& username);

    // All users
    void viewByCategory();
    void searchProducts();
    void sortProducts();
};