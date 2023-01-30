#pragma once

#include "Item.hpp"
#include <Hashmap.hpp>
#include <EventLoop.hpp>

class Cart
{
    double m_total;
    Hashmap<uint32_t, Item*> m_cart;

public:
    Cart();
    ~Cart();
    
    void add(Event* evt);
    void remove(Event* evt);
    void checkout(Event* evt);
    void view() const;
    double getTotal();
};