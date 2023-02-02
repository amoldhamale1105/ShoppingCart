#pragma once

#include "EnumTypes.h"
#include <EventLoop.hpp>
#include <String.hpp>
#include <Pair.hpp>
#include <cstdint>

class Product
{
    uint32_t m_id, m_stock;
    double m_price;
    String m_name;
    ProductType m_type;

    void manageProduct(Event*);

public:
    Product();
    Product(const uint32_t& id, const String& name, const double& price, const uint32_t& stock);
    Product(const uint32_t& id, const String& name, const ProductType& type, const double& price, const uint32_t& stock);
    virtual ~Product();

    uint32_t getID() const;
    String getName() const;
    ProductType getType() const;
    double getPrice() const;
};
