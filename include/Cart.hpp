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
    
    void add(EventLoop::Event* evt);
    void remove(EventLoop::Event* evt);
    void cartActions(EventLoop::Event* evt);
    void applyDiscount(EventLoop::Event* evt);
    double getTotal() const;
};
