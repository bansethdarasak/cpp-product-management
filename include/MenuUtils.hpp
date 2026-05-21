#pragma once
#include <string>
#include <vector>
#include "Product.hpp"

using std::string;
using std::vector;

void printMenu(const vector<string>& items);
void printProducts(const vector<Product>& products);
void clearScreen();
void pauseScreen();