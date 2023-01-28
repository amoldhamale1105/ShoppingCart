#include "Inventory.h"

bool operator==(const String &str1, const String &str2)
{
    return str1 == str2;
}

Inventory::Inventory()
{
    giftCards.insert("DIWALITYUPJK", 32.5);
    giftCards.insert("XMASTO5FG754", 25.2);
}