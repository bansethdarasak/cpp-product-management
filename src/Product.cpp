#include "Product.hpp"

Product::Product(int id, string name, string category, double price, int stock, string owner)
    : id(id), name(name), category(category), price(price), stock(stock), owner(owner) {}

int    Product::getId()       const { return id; }
string Product::getName()     const { return name; }
string Product::getCategory() const { return category; }
double Product::getPrice()    const { return price; }
int    Product::getStock()    const { return stock; }
string Product::getOwner()    const { return owner; }

void Product::setName(string name)         { this->name = name; }
void Product::setCategory(string category) { this->category = category; }
void Product::setPrice(double price)       { this->price = price; }
void Product::setStock(int stock)          { this->stock = stock; }