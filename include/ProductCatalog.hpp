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
    string          dataFile;
    string          pendingFile;

    int getNextId() const;

public:
    ProductCatalog(string dataFile, string pendingFile);
    ~ProductCatalog();

    // Admin only
    void addProduct(const string& username);
    void updateProduct();
    void deleteProduct();
    void reviewPending();
    void deleteByOwner(const string& username);
    void checkLowStock();
    void viewAllUsers(const vector<class User*>& users);

    // User only
    void submitProduct(const string& username);
    void takeOutProduct(const string& username);
    void viewMyProducts(const string& username);
    void requestUpdateProduct(const string& username); 

    // All users
    void viewByCategory();
    void searchProducts();
    void sortProducts();

    // Paginated view (shared helper)
    void viewPaginated(const vector<Product>& list, const string& title = "PRODUCTS");

    // Returns count of products with stock < 3 (used for login alert)
    int countLowStock() const;
};