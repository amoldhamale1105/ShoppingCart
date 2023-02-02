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
    Hashmap<std::string,float> coupons;
    Vector<Product*> database;
    Inventory();
    ~Inventory();
};
