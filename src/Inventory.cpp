#include "Inventory.h"

bool operator==(const String &str1, const String &str2)
{
    return str1.length() == str2.length() && strncmp(str1.c_str(), str2.c_str(), str1.length()) == 0;
}

Inventory::Inventory()
{
    coupons.insert("DIWALITYUPJK", 32.5);
    coupons.insert("GINDFESTPPYD", 48);
    coupons.insert("05XMASYPJTY4", 34);
    coupons.insert("01PADWAQXV6L", 40);
    coupons.insert("06MRFHSFI7RH", 25);

    Vector<Product*> category;
    category.push_back(new Product(14561, "Rice", ProductType::GROCERY, 106, 27));
    category.push_back(new Product(43242, "Wheat flour", ProductType::GROCERY, 95, 58));
    category.push_back(new Product(34732, "Oats", ProductType::GROCERY, 210, 60));
    category.push_back(new Product(28324, "Peanuts", ProductType::GROCERY, 35, 200));
    category.push_back(new Product(19843, "Salt", ProductType::GROCERY, 41.7, 250));

    database.push_back(category);
    category.clear();

    category.push_back(new Product(14567, "Apple", ProductType::FRUITS, 45.5, 74));
    category.push_back(new Product(14567, "Guava", ProductType::FRUITS, 21.8, 175));
    category.push_back(new Product(14567, "Orange", ProductType::FRUITS, 63, 87));
    category.push_back(new Product(14567, "Mango", ProductType::FRUITS, 130.6, 44));
    category.push_back(new Product(14567, "Watermelon", ProductType::FRUITS, 45.5, 74));

    database.push_back(category);
    category.clear();

    category.push_back(new Product(54745, "Spinach", ProductType::VEGETABLES, 34, 58));
    category.push_back(new Product(54745, "Brinjal", ProductType::VEGETABLES, 61.4, 98));
    category.push_back(new Product(54745, "Potato", ProductType::VEGETABLES, 31.9, 125));
    category.push_back(new Product(54745, "Capsicum", ProductType::VEGETABLES, 47, 233));
    category.push_back(new Product(54745, "Onion", ProductType::VEGETABLES, 74.2, 330));
    
    database.push_back(category);
    category.clear();

    category.push_back(new Product(35345, "Milk", ProductType::DAIRY, 50.8, 230));
    category.push_back(new Product(35345, "Butter", ProductType::DAIRY, 88, 14));
    category.push_back(new Product(35345, "Cheese", ProductType::DAIRY, 94.6, 54));
    category.push_back(new Product(35345, "Buttermilk", ProductType::DAIRY, 49.34, 97));
    category.push_back(new Product(35345, "Curd", ProductType::DAIRY, 53.87, 66));

    database.push_back(category);
    category.clear();

    category.push_back(new Product(16546, "iPhone", ProductType::ELECTRONICS, 99956.44, 16));
    category.push_back(new Product(16546, "iPad", ProductType::ELECTRONICS, 64342.21, 31));
    category.push_back(new Product(16546, "AirPods", ProductType::ELECTRONICS, 22698.7, 83));
    category.push_back(new Product(16546, "Mouse", ProductType::ELECTRONICS, 450, 72));
    category.push_back(new Product(16546, "Monitor", ProductType::ELECTRONICS, 5200.8, 29));

    database.push_back(category);
    category.clear();

    category.push_back(new Product(12312, "Blueprint", ProductType::BOOKS, 352.52, 95));
    category.push_back(new Product(12312, "Animal farm", ProductType::BOOKS, 102, 430));
    category.push_back(new Product(12312, "Steve Jobs", ProductType::BOOKS, 699.99, 172));
    category.push_back(new Product(12312, "Sapiens", ProductType::BOOKS, 449.5, 75));
    category.push_back(new Product(12312, "Poor Economics", ProductType::BOOKS, 410.85, 234));

    database.push_back(category);
    category.clear();

    category.push_back(new Product(67657, "Chair", 2300, 15));
    category.push_back(new Product(67657, "Table", 3866.99, 47));
    category.push_back(new Product(67657, "Water filter", 7800, 22));
    category.push_back(new Product(67657, "Carpet", 2395.77, 170));
    category.push_back(new Product(67657, "Pen", 120, 580));

    database.push_back(category);
}

Inventory::~Inventory()
{
    Vector<Product*> category;
    while (!database.isEmpty())
    {
        category = database.pop_back();
        while (!category.isEmpty())
        {
            delete category.pop_back();
        }
    }
}
