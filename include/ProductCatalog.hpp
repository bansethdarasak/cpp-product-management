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
    void addProduct();
    void updateProduct();
    void deleteProduct();
    void reviewPending();     // approve / reject user submissions

    // All users
    void submitProduct();     // user submits — goes to pending
    void viewByCategory();
    void searchProducts();
    void sortProducts();
};