#include "Cart.hpp"

Cart::Cart() : m_total(0)
{
    EventLoop::RegisterEvent("AddCart", std::bind(&Cart::add, this, std::placeholders::_1));
    EventLoop::RegisterEvent("RemCart", std::bind(&Cart::remove, this, std::placeholders::_1));
    EventLoop::RegisterEvent("Checkout", std::bind(&Cart::checkout, this, std::placeholders::_1));
}

Cart::~Cart()
{
}

void Cart::add(Event *evt)
{
    Pair<Product*, int>* productInfo = static_cast<Pair<Product*, int>*>(evt->getData());
    uint32_t productID = productInfo->first->getID();

    if (!m_cart.contains(productID))
        m_cart.insert(productID, new Item(productInfo->first));
    
    m_cart.at(productID)->add(productInfo->second);
    std::cout<<"Added "<<productInfo->second<<" units of "<<productInfo->first->getName()<<" to cart"<<std::endl;
    EventLoop::TriggerEvent("CleanMem", productInfo);
    EventLoop::TriggerEvent("Shop");
}

void Cart::remove(Event *evt)
{
    Pair<Product*, int>* productInfo = static_cast<Pair<Product*, int>*>(evt->getData());
    uint32_t productID = productInfo->first->getID();

    if (!m_cart.contains(productID)){
        std::cout<<productInfo->first->getName()<<" not present in cart"<<std::endl;
        EventLoop::TriggerEvent("CleanMem", productInfo);
        EventLoop::TriggerEvent("Shop");
        return;
    }
    if (productInfo->second == -1){
        productInfo->second = m_cart.at(productID)->getQuantity();
        m_cart.erase(productID);
        std::cout<<"Removed "<<productInfo->first->getName()<<" from cart"<<std::endl;
        EventLoop::TriggerEvent("RemItem", productInfo);
        return;
    }
    uint32_t currQuantity = m_cart.at(productID)->getQuantity();
    if (productInfo->second > currQuantity){
        std::cout<<"There are only "<<currQuantity<<" units of "<<productInfo->first->getName()<<" in the cart"<<std::endl;
        productInfo->second = currQuantity;
    }
    std::cout<<"Removed "<<productInfo->second<<" units of "<<productInfo->first->getName()<<" from cart"<<std::endl;
    m_cart.at(productID)->remove(productInfo->second);
    if (m_cart.at(productID)->getQuantity() == 0)
        m_cart.erase(productID);
    
    EventLoop::TriggerEvent("RemItem", productInfo);
}

void Cart::checkout(Event *evt)
{
}

void Cart::view() const
{
}

double Cart::getTotal()
{
    Vector<uint32_t> ids = m_cart.keys();
    while (!ids.isEmpty())
    {
        m_total += m_cart.at(ids.pop_back())->getAmount();
    }
    return m_total;
}
