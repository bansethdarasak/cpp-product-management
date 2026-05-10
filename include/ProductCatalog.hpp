#pragma once
#include <string>
#include <vector>
#include "Product.hpp"
#include "User.hpp"

using std::string;
using std::vector;

class ProductCatalog {
private:
    vector<Product> products;
    int             nextId;
    string          dataFile;

public:
    ProductCatalog(string dataFile);
    ~ProductCatalog();

    void addProduct();
    void viewByCategory();
    void updateProduct();
    void deleteProduct();
    void searchProducts();
    void sortProducts();
};
