#include "Cart.hpp"
#include <iomanip>

Cart::Cart() : m_total(0), m_billTotal(m_total)
{
    EventLoop::RegisterEvent("AddCart", std::bind(&Cart::add, this, std::placeholders::_1));
    EventLoop::RegisterEvent("RemCart", std::bind(&Cart::remove, this, std::placeholders::_1));
    EventLoop::RegisterEvents({"View","Checkout","CartPay"}, std::bind(&Cart::cartActions, this, std::placeholders::_1));
    EventLoop::RegisterEvent("Discount", std::bind(&Cart::applyDiscount, this, std::placeholders::_1));
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
    if (evt->getData() == nullptr){
        if (m_cart.isEmpty())
            std::cout<<"Cart is already empty"<<std::endl;
        else{
            m_total = 0;
            m_cart.clear();
            std::cout<<"Cart emptied. Please continue shopping or enter (h)elp for usage details"<<std::endl;
        }
        EventLoop::TriggerEvent("Shop");
        return;
    }

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
        delete m_cart.at(productID);
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

void Cart::cartActions(Event *evt)
{
    String evtName = evt->getName().c_str();

    if (evtName == "View"){
        String* userName = static_cast<String*>(evt->getData());
        if (m_cart.isEmpty())
            std::cout<<"Cart is empty. Enter (h)elp for usage details"<<std::endl;
        else
            view(*userName);
        EventLoop::TriggerEvent("Shop");
    }
    else if (evtName == "Checkout"){
        String* userName = static_cast<String*>(evt->getData());
        if (m_cart.isEmpty()){
            std::cout<<"Failed to checkout. Proceed to add items to cart first"<<std::endl;
            EventLoop::TriggerEvent("Shop");
            return;
        }
        m_billTotal = m_total;
        m_usedCoupons.clear();
        view(*userName);
        std::cout<<"\n#### Total amount payable: "<<m_billTotal<<" ####"<<std::endl;
        EventLoop::TriggerEvent("Pay");
    }
    else if (evtName == "CartPay"){
        Pair<PaymentMode,double>* paymentData = static_cast<Pair<PaymentMode,double>*>(evt->getData());
        if (paymentData->second == -1)
            paymentData->second = m_billTotal;
        switch (paymentData->first)
        {
        case PaymentMode::CASH:
        {
            double diff = std::abs(m_billTotal - paymentData->second);
            if (paymentData->second >= m_billTotal){
                std::cout<<"Thank you! Your change after paying "<<std::fixed<<std::setprecision(2)<<paymentData->second<<" in cash: "<<diff<<std::endl;
                std::cout<<"Your order will be delivered soon. Please continue shopping"<<std::endl;
                m_total = 0;
                m_cart.clear();
                EventLoop::TriggerEvent("Shop");
            }
            else{
                m_billTotal -= paymentData->second;
                std::cout<<"Thank you for the payment. Remaining amount payable: "<<m_billTotal<<std::endl;
                EventLoop::TriggerEvent("Pay");
            }
            break;
        }
        case PaymentMode::ONLINE:
            if (paymentData->second > m_billTotal)
                paymentData->second = m_billTotal;
            m_billTotal -= paymentData->second;
            std::cout<<"Thank you! INR "<<std::fixed<<std::setprecision(2)<<paymentData->second<<" will be deducted from your SB wallet"<<std::endl;
            if (m_billTotal > 0){
                std::cout<<"Remaining amount payable: "<<m_billTotal<<std::endl;
                EventLoop::TriggerEvent("Pay");
            }
            else{
                std::cout<<"Your order will be delivered soon. Please continue shopping"<<std::endl;
                m_total = 0;
                m_cart.clear();
                EventLoop::TriggerEvent("Shop");
            }
        default:
            break;
        }
    }
}

void Cart::applyDiscount(Event *evt)
{
    String evtName = evt->getName().c_str();
    if (evtName == "Discount"){
        Pair<String, float>* discountData = static_cast<Pair<String,float>*>(evt->getData());
        ProductType type = ProductType::ALL;
        if (m_usedCoupons.position(discountData->first) != -1){
            std::cout<<"This coupon has already been applied. Try another or proceed to payment"<<std::endl;
            EventLoop::TriggerEvent("Pay");
            return;
        }
        if (std::isdigit(discountData->first.at(0)) && std::isdigit(discountData->first.at(1))){
            char catStr[3];
            strncpy(catStr, discountData->first.c_str(), 2); 
            catStr[2] = '\0';
            type = static_cast<ProductType>(std::stoi(catStr));
        }
        double discountAmt{0};
        if (type != ProductType::ALL){
            Vector<uint32_t> ids = m_cart.keys();
            for(auto i = 0; i < ids.size(); i++)
            {
                Item* item = m_cart.at(ids.at(i));
                if (item->getType() == type){
                    discountAmt += (item->getAmount() * discountData->second/100);
                }
            }
        }
        else
            discountAmt = m_billTotal * discountData->second/100;
        
        m_billTotal -= discountAmt;
        m_usedCoupons.push_back(discountData->first);
        std::cout<<"Coupon applied successfully. Amount payable after applying discount: "<<m_billTotal<<std::endl;
        EventLoop::TriggerEvent("Pay");
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
