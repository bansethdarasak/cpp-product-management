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
    string owner;

public:
    Product(int id, string name, string category, double price, int stock, string owner = "admin");

    int    getId()       const;
    string getName()     const;
    string getCategory() const;
    double getPrice()    const;
    int    getStock()    const;
    string getOwner()    const;

    void setName(string name);
    void setCategory(string category);
    void setPrice(double price);
    void setStock(int stock);
};