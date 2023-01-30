#pragma once
#include <Inventory.h>
#include <String.hpp>
#include <Pair.hpp>
#include <EventLoop.hpp>

class Shell
{
    String m_input, m_userName;
    Inventory m_inventory;
    bool m_exitFlag, m_paymentFlag;

    void getUserInput(Event* evt);
    void cleanup(Event* evt);
    int findProduct(const String& name);
    void usage();

public:
    Shell(const String& user);
    ~Shell();
};