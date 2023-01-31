#include "Cart.hpp"
#include <iomanip>

Cart::Cart() : m_total(0)
{
    EventLoop::RegisterEvent("AddCart", std::bind(&Cart::add, this, std::placeholders::_1));
    EventLoop::RegisterEvent("RemCart", std::bind(&Cart::remove, this, std::placeholders::_1));
    EventLoop::RegisterEvents({"View","Checkout"}, std::bind(&Cart::viewAndCheckout, this, std::placeholders::_1));
}

Cart::~Cart()
{
    Vector<uint32_t> ids = m_cart.keys();
    while (!ids.isEmpty())
    {
        delete m_cart.at(ids.pop_back());
    }
}

void Cart::add(Event *evt)
{
    Pair<Product*, int>* productInfo = static_cast<Pair<Product*, int>*>(evt->getData());
    uint32_t productID = productInfo->first->getID();

    if (!m_cart.contains(productID))
        m_cart.insert(productID, new Item(productInfo->first));

    double prevItemAmt = m_cart.at(productID)->getAmount();
    m_cart.at(productID)->add(productInfo->second);
    m_total += (m_cart.at(productID)->getAmount() - prevItemAmt);
    std::cout<<"Added "<<productInfo->second<<" units of "<<productInfo->first->getName()<<" to cart"<<std::endl;
    EventLoop::TriggerEvent("CleanProductInfo", productInfo);
    EventLoop::TriggerEvent("Shop");
}

void Cart::remove(Event *evt)
{
    Pair<Product*, int>* productInfo = static_cast<Pair<Product*, int>*>(evt->getData());
    uint32_t productID = productInfo->first->getID();

    if (!m_cart.contains(productID)){
        std::cout<<productInfo->first->getName()<<" not present in cart"<<std::endl;
        EventLoop::TriggerEvent("CleanProductInfo", productInfo);
        EventLoop::TriggerEvent("Shop");
        return;
    }
    if (productInfo->second == -1){
        productInfo->second = m_cart.at(productID)->getQuantity();
        m_total -= m_cart.at(productID)->getAmount();
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
    double prevItemAmt = m_cart.at(productID)->getAmount();
    m_cart.at(productID)->remove(productInfo->second);
    if (m_cart.at(productID)->getQuantity() == 0){
        delete m_cart.at(productID);
        m_cart.erase(productID);
    }
    m_total -= (prevItemAmt - m_cart.at(productID)->getAmount());
    std::cout<<"Removed "<<productInfo->second<<" units of "<<productInfo->first->getName()<<" from cart"<<std::endl;

    EventLoop::TriggerEvent("RemItem", productInfo);
}

void Cart::viewAndCheckout(Event *evt)
{
    String evtName = evt->getName().c_str();
    String* userName = static_cast<String*>(evt->getData());

    if (evtName == "View"){
        if (m_cart.isEmpty())
            std::cout<<"Cart is empty. Enter (h)elp for usage details"<<std::endl;
        else
            view(*userName);
        EventLoop::TriggerEvent("Shop");
    }
}

void Cart::view(const String& userName) const
{
    int serialNum{1};

    String dottedLine = "----------------------";
    String header = dottedLine + userName + "\'s Cart" + dottedLine;
    std::cout<<header<<std::endl;
    std::cout<<std::setw(10)<<std::left<<"SR."<<std::setw(20)<<"NAME"<<std::setw(10)<<"QUANTITY"<<std::setw(12)<<std::right<<"AMOUNT"<<std::endl;
    Vector<uint32_t> ids = m_cart.keys();

    while (!ids.isEmpty())
    {
        Item* item = m_cart.at(ids.pop_back());
        std::cout<<std::setw(10)<<std::left<<serialNum++
                 <<std::setw(20)<<item->getName()
                 <<std::setw(10)<<item->getQuantity()
                 <<std::fixed<<std::setprecision(2)<<std::setw(12)<<std::right<<item->getAmount()
                 <<std::endl;
    }
    
    for(auto i = 0; i < header.length(); i++)
    {
        std::cout<<"-";
    }
    std::cout<<std::endl<<std::setw(40)<<std::left<<"TOTAL"<<std::setw(12)<<std::right<<m_total<<std::endl;
}

double Cart::getTotal() const
{
    return m_total;
}
