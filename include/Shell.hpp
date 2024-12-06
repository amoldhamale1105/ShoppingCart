#pragma once
#include <Inventory.h>
#include <String.hpp>
#include <Pair.hpp>
#include <EventLoop.h>

class Shell
{
    Vector<String> m_commandVec;
    String m_input, m_userName, m_command;
    Inventory m_inventory;
    bool m_exitFlag, m_emptyCart;

    void getUserInput(EventLoop::Event* evt);
    void cleanup(EventLoop::Event* evt);
    int findProduct(const String& name);
    Product* getProduct(const int& index);
    void usage();

public:
    Shell(const String& user);
    ~Shell();
};
