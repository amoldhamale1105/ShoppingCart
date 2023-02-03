#include "Inventory.h"

bool operator==(const String &str1, const String &str2)
{
    std::string stdStr1 = str1.c_str();
    std::string stdStr2 = str2.c_str();
    return stdStr1 == stdStr2;
}

Inventory::Inventory()
{
    coupons.insert("DIWALITYUPJK", 32.5);
    coupons.insert("GINDFESTPPYD", 55);
    coupons.insert("05XMASYPJTY4", 14);
    coupons.insert("01PADWAQXV6L", 40);

    database.push_back(new Product(14561, "Rice", ProductType::GROCERY, 106, 27));
    database.push_back(new Product(14567, "Apple", ProductType::FRUITS, 45.5, 74));
    database.push_back(new Product(54745, "Spinach", ProductType::VEGETABLES, 34, 58));
    database.push_back(new Product(35345, "Milk", ProductType::DAIRY, 50.8, 230));
    database.push_back(new Product(16546, "iPhone", ProductType::ELECTRONICS, 99956.44, 16));
    database.push_back(new Product(12312, "Blueprint", ProductType::BOOKS, 352.52, 95));
    database.push_back(new Product(67657, "Chair", 2300, 15));
}

Inventory::~Inventory()
{
    while (!database.isEmpty())
    {
        delete database.pop_back();
    }
}
