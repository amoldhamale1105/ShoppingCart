#include "Item.hpp"

Item::Item(Product *product) : m_product(product), m_quantity(0), m_amount(0)
{
}

Item::~Item()
{
}

void Item::add(const uint32_t &quantity)
{
    m_quantity += quantity;
    m_amount = m_product->getPrice()*m_quantity;
}

void Item::remove(const uint32_t &quantity)
{
    m_quantity = quantity > m_quantity ? 0 : m_quantity - quantity;
    m_amount = m_product->getPrice()*m_quantity;
}

String Item::getName() const
{
    return m_product->getName();
}

ProductType Item::getType() const
{
    return m_product->getType();
}

uint32_t Item::getQuantity() const
{
    return m_quantity;
}

double Item::getAmount() const
{
    return m_amount;
}
