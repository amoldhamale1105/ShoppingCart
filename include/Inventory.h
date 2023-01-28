#pragma once

#include "Product.hpp"
#include <Vector.hpp>
#include <Hashmap.hpp>

bool operator==(const String& str1, const String& str2);

namespace std {
    template <>
    struct hash<String>
    {
        size_t operator()(const String& str) const
        {
            const int prime1{54059};
            const int prime2{76963};
            size_t strHash = 37;

            const char* strArr = str.c_str();

            while (*strArr)
            {
                strHash = (strHash * prime1) ^ (strArr[0] * prime2);
                strArr++;
            }
            
            return strHash;
        }
    };
};

struct Inventory
{
    Hashmap<String,float> giftCards;
    Inventory();

    Vector<Product> database = {
        { 14561, "Rice", ProductType::GROCERY, 106, 27 },
        { 14567, "Apple", ProductType::FRUITS, 45.5, 74 },
        { 35345, "Milk", ProductType::DAIRY, 50.8, 230 },
        { 16546, "iPhone", ProductType::ELECTRONICS, 99956.44, 16 },
        { 12312, "Blueprint", ProductType::BOOKS, 352.52, 95 },
        { 67657, "Chair", 2300, 15 }
    };
};
