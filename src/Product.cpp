#include <Product.hpp>

Product::Product() : m_id(0), m_name(""), m_type(ProductType::INVALID), m_price(0), m_stock(0)
{
    EventLoop::RegisterEvents({"AddItem","RemItem"}, std::bind(&Product::HandleReceivedEvent, this, std::placeholders::_1));
}

Product::Product(const uint32_t &id, const String &name, const double &price, const uint32_t &stock) : 
    m_id(id), m_name(name), m_type(ProductType::MISC), m_price(price), m_stock(stock)
{
    EventLoop::RegisterEvents({"AddItem","RemItem"}, std::bind(&Product::HandleReceivedEvent, this, std::placeholders::_1));
}

Product::Product(const uint32_t &id, const String &name, const ProductType &type, const double &price, const uint32_t &stock) : 
    m_id(id), m_name(name), m_type(type), m_price(price), m_stock(stock)
{
    EventLoop::RegisterEvents({"AddItem","RemItem"}, std::bind(&Product::HandleReceivedEvent, this, std::placeholders::_1));
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

void Product::HandleReceivedEvent(Event *evt)
{
    Pair<Product*, int>* productInfo = static_cast<Pair<Product*, int>*>(evt->getData());
    if (m_name != productInfo->first->getName())
        return;
    String evtName = evt->getName().c_str();
    
    if (evtName == "AddItem"){
        if (m_stock == 0){
            std::cout<<"Sorry, "<<m_name<<" is currently out of stock. Please try later"<<std::endl;
            EventLoop::TriggerEvent("CleanMem", productInfo);
            EventLoop::TriggerEvent("Shop");
            return;
        }
        if (productInfo->second > m_stock){
            std::cout<<"Only "<<m_stock<<" out of "<<productInfo->second<<" left in stock"<<std::endl;
            productInfo->second = m_stock;
        }
        m_stock -= productInfo->second;
        EventLoop::TriggerEvent("AddCart", productInfo);
    }
    else if (evtName == "RemItem"){
        m_stock += productInfo->second;
        EventLoop::TriggerEvent("CleanMem", productInfo);
        EventLoop::TriggerEvent("Shop");
    }
}
