#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class Product {
private:
    int    id;
    string name;
    string category;
    double price;
    int    stock;

public:
    Product(int id, string name, string category, double price, int stock);

    int    getId()       const;
    string getName()     const;
    string getCategory() const;
    double getPrice()    const;
    int    getStock()    const;

    void setName(string name);
    void setCategory(string category);
    void setPrice(double price);
    void setStock(int stock);
};
