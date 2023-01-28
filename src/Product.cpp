#include <Product.hpp>

Product::Product() :
    m_id(0), m_name(""), m_type(ProductType::INVALID), m_price(0), m_stock(0)
{
}

Product::Product(const uint32_t &id, const String &name, const double &price, const uint32_t &stock) : 
    m_id(id), m_name(name), m_type(ProductType::MISC), m_price(price), m_stock(stock)
{
}

Product::Product(const uint32_t &id, const String &name, const ProductType &type, const double &price, const uint32_t &stock) : 
    m_id(id), m_name(name), m_type(type), m_price(price), m_stock(stock)
{
}

Product::~Product()
{
}

uint32_t Product::getID() const
{
    return m_id;
}

String Product::getName() const
{
    return m_name;
}

ProductType Product::getType() const
{
    return m_type;
}

double Product::getPrice() const
{
    return m_price;
}

uint32_t Product::getStock() const
{
    return m_stock;
}

bool Product::inStock() const
{
    return m_stock > 0;
}

void Product::addStock(const uint32_t &quantity)
{
    m_stock += quantity;
}

void Product::reduceStock(const uint32_t &quantity)
{
    m_stock = quantity > m_stock ? 0 : m_stock - quantity;
}
