#pragma once
#include "Product.hpp"
#include <cstdint>

class Item
{
    Product* m_product;
    uint32_t m_quantity;
    double m_amount;

public:
    Item(Product* product);
    ~Item();

    void add(const uint32_t& quantity = 1);
    void remove(const uint32_t& quantity = 1);
    String getName() const;
    ProductType getType() const;
    uint32_t getQuantity() const;
    double getAmount() const;
};