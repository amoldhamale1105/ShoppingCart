#include <iostream>
#include <Shell.hpp>
#include <Cart.hpp>

int main(int argc, char **argv)
{
    Shell cartShell(argc > 1 ? argv[1] : "Customer");

    Cart shoppingCart;

    /* Initialization event to start the shopping cart shell */
    EventLoop::TriggerEvent("Shop");

    EventLoop::Run();

    return 0;
}
