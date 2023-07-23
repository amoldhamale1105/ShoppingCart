#pragma once

#include "Item.hpp"
#include <Hashmap.hpp>
#include <EventLoop.h>
#include <List.hpp>

class Cart
{
    double m_total, m_billTotal;
    Hashmap<uint32_t, Item*> m_cart;
    List<String> m_usedCoupons;

    void view(const String& userName) const;
    void emptyCart();

public:
    Cart();
    ~Cart();
    
    void add(Event* evt);
    void remove(Event* evt);
    void cartActions(Event* evt);
    void applyDiscount(Event* evt);
    double getTotal() const;
};