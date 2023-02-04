#include "Shell.hpp"

Shell::Shell(const String &user) : m_userName(user), m_exitFlag(false), m_emptyCart(false)
{
    EventLoop::RegisterEvents({"Shop","Pay"}, std::bind(&Shell::getUserInput, this, std::placeholders::_1));
    EventLoop::RegisterEvents({"CleanProductInfo","CleanCouponInfo","CleanPaymentData"}, std::bind(&Shell::cleanup, this, std::placeholders::_1));

    std::cout<<"--------Welcome to Small Basket shopping cart shell--------\n"<<std::endl;
    usage();
}

Shell::~Shell()
{
}

void Shell::getUserInput(Event *evt)
{
    std::cout<<m_userName<<"@SmallBasket:~# ";
    std::cin >> m_input;
    m_input.trim();
    if (m_input.empty()){
        EventLoop::TriggerEvent(evt->getName());
        return;
    }
    m_commandVec = m_input.split(' ');
    m_command = m_commandVec.front();
    String evtName = evt->getName().c_str();

    if (evtName == "Shop"){
        if (m_command == "a" || m_command == "add"){
            if (m_exitFlag)
                m_exitFlag = false;
            if (m_commandVec.size() < 3){
                std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
            String productArg = m_commandVec.at(1);
            bool isProductIndex{true};
            for (auto i = 0; i < productArg.length(); i++)
            {
                if (!std::isdigit(productArg.at(i))){
                    isProductIndex = false;
                    break;
                }
            }
            int productIndex = isProductIndex ? std::stoi(productArg.c_str()) : findProduct(productArg);
            if (productIndex == -1 || productIndex >= m_inventory.database.size()){
                std::cout<<"Invalid product name/index. Given product does not exist"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }

            String quantityArg = m_commandVec.at(2);
            bool isQuantityValid{true};
            for (auto i = 0; i < quantityArg.length(); i++)
            {
                if (!std::isdigit(quantityArg.at(i))){
                    isQuantityValid = false;
                    break;
                }
            }
            if (!isQuantityValid){
                std::cout<<"Please provide valid numeric value for quantity. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }

            EventLoop::TriggerEvent("AddItem", new Pair<Product*,int>(m_inventory.database[productIndex], std::stoi(quantityArg.c_str())));
        }
        else if (m_command == "r" || m_command == "remove"){
            if (m_exitFlag)
                m_exitFlag = false;
            if (m_commandVec.size() < 2){
                std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
            String productArg = m_commandVec.at(1);
            bool isProductIndex{true};
            for (auto i = 0; i < productArg.length(); i++)
            {
                if (!std::isdigit(productArg.at(i))){
                    isProductIndex = false;
                    break;
                }
            }
            int productIndex = isProductIndex ? std::stoi(productArg.c_str()) : findProduct(productArg);
            if (productIndex == -1 || productIndex >= m_inventory.database.size()){
                std::cout<<"Invalid product name/index. Given product does not exist"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
            int quantity{-1};
            if (m_commandVec.size() > 2){
                String quantityArg = m_commandVec.at(2);
                bool isQuantityValid{true};
                for (auto i = 0; i < quantityArg.length(); i++)
                {
                    if (!std::isdigit(quantityArg.at(i))){
                        isQuantityValid = false;
                        break;
                    }
                }
                if (!isQuantityValid){
                    std::cout<<"Please provide valid numeric value for quantity. Enter (h)elp for usage details"<<std::endl;
                    EventLoop::TriggerEvent("Shop");
                    return;
                }
                quantity = std::stoi(quantityArg.c_str());
            }

            EventLoop::TriggerEvent("RemCart", new Pair<Product*,int>(m_inventory.database[productIndex], quantity));
        }
        else if (m_command == "u" || m_command == "unload"){
            std::cout<<"Are you sure? All items added in the cart will be removed"<<std::endl;
            m_emptyCart = true;
            EventLoop::TriggerEvent("Shop");
        }
        else if (m_command == "h" || m_command == "help"){
            usage();
            EventLoop::TriggerEvent("Shop");
            return;
        }
        else if (m_command == "e" || m_command == "exit"){
            std::cout<<"Are you sure? Your current session and cart status will be lost"<<std::endl;
            m_exitFlag = true;
            EventLoop::TriggerEvent("Shop");
            return;
        }
        else if (m_command == "c" || m_command == "checkout"){
            EventLoop::TriggerEvent("Checkout", &m_userName);
        }
        else if (m_command == "y" || m_command == "yes"){
            if (m_exitFlag){
                std::cout<<"Thank you for shopping with us. See you again!"<<std::endl;
                EventLoop::Halt();
                return;
            }
            else if (m_emptyCart){
                EventLoop::TriggerEvent("RemCart", nullptr);
                m_emptyCart = false;
                return;
            }
            std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Shop");
            return;
        }
        else if (m_command == "n" || m_command == "no"){
            if (m_exitFlag){
                std::cout<<"Good call! Please continue shopping and enter (h)elp if needed"<<std::endl;
                m_exitFlag = false;
            }
            else if (m_emptyCart){
                std::cout<<"Cart status retained. Please continue shopping"<<std::endl;
                m_emptyCart = false;
            }
            else
                std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Shop");
            return;                
        }
        else if (m_command == "v" || m_command == "view"){
            EventLoop::TriggerEvent("View", &m_userName);
        }
        else{
            std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Shop");
            return;
        }
    }
    else if (evtName == "Pay"){
        if (m_command == "d" || m_command == "discount"){
            if (m_commandVec.size() < 2){
                std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Pay");
                return;
            }
            String couponCode = m_commandVec.at(1);
            if (m_inventory.coupons.contains(couponCode)){
                EventLoop::TriggerEvent("Discount", new Pair<String,float>(couponCode, m_inventory.coupons.at(couponCode)));
                return;
            }
            std::cout<<"Invalid coupon code. Please try again or enter (n)o to proceed to payment"<<std::endl;
            EventLoop::TriggerEvent("Pay");
        }
        else if (m_command == "p" || m_command == "pay"){
            if (m_commandVec.size() < 2){
                std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Pay");
                return;
            }
            String payModeStr = m_commandVec.at(1);
            PaymentMode payMode;
            if (payModeStr == "c" || payModeStr == "cash")
                payMode = PaymentMode::CASH;
            else if (payModeStr == "o" || payModeStr == "online")
                payMode = PaymentMode::ONLINE;
            else{
                std::cout<<"Invalid payment mode. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Pay");
                return;
            }
            
            double amount{-1};

            if (m_commandVec.size() > 2){
                String amtArg = m_commandVec.at(2);
                bool isAmtValid{true}, decimalPoint{false};
                for (auto i = 0; i < amtArg.length(); i++)
                {
                    if (amtArg.at(i) == '.'){
                        if (!decimalPoint){
                            decimalPoint = true;
                            continue;   
                        }
                        else{
                            isAmtValid = false;
                            break;
                        }
                    }
                    if (!std::isdigit(amtArg.at(i))){
                        isAmtValid = false;
                        break;
                    }
                }
                if (!isAmtValid){
                    std::cout<<"Please provide valid numeric value for amount. Enter (h)elp for usage details"<<std::endl;
                    EventLoop::TriggerEvent("Pay");
                    return;
                }
                amount = std::stod(amtArg.c_str());
            }

            EventLoop::TriggerEvent("CartPay", new Pair<PaymentMode,double>(payMode, amount));
        }
        else if (m_command == "h" || m_command == "help"){
            usage();
            EventLoop::TriggerEvent("Pay");
        }
        else if (m_command == "s" || m_command == "stop"){
            std::cout<<"Checkout aborted. You can now continue shopping. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Shop");
        }
        else if (m_command == "e" || m_command == "exit"){
            std::cout<<"Are you sure? Your checkout and cart status will be lost"<<std::endl;
            m_exitFlag = true;
            EventLoop::TriggerEvent("Pay");
            return;
        }
        else if (m_command == "y" || m_command == "yes"){
            if (m_exitFlag){
                std::cout<<"Thank you for shopping with us. See you again!"<<std::endl;
                EventLoop::Halt();
                return;
            }
            std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Pay");
            return;
        }
        else if (m_command == "n" || m_command == "no"){
            if (m_exitFlag){
                std::cout<<"Great. Please proceed with checkout or return to shopping with (s)top command"<<std::endl;
                m_exitFlag = false;
            }
            else
                std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Pay");
            return;                
        }
        else{
            std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
            EventLoop::TriggerEvent("Pay");
        }
    }
}

void Shell::cleanup(Event *evt)
{
    void* payload = evt->getData();
    if (payload != nullptr){
        String evtName = evt->getName().c_str();

        if (evtName == "CleanProductInfo")
            delete static_cast<Pair<Product*,int>*>(payload);
        else if (evtName == "CleanCouponInfo")
            delete static_cast<Pair<String,float>*>(payload);
        else if (evtName == "CleanPaymentData")
            delete static_cast<Pair<PaymentMode,double>*>(payload);
    }
}

int Shell::findProduct(const String &name)
{
    int targetIndex{-1};
    int totalItems = m_inventory.database.size();
    for(auto i = 0; i < totalItems; i++)
    {
        if (strcasecmp(m_inventory.database.at(i)->getName().c_str(), name.c_str()) == 0){
            targetIndex = i;
            break;
        }
    }
    return targetIndex;
}

void Shell::usage()
{
    std::cout<<"Shopping commands:\n\
        (a)dd <category-index | product-name> <quantity>\n\
        (r)emove <category-index | product-name> [quantity]\n\
        (u)nload cart\n\
        (v)iew cart\n\
        (c)heckout\nCheckout mode commands:\n\
        (d)iscount <coupon-code>\n\
        (p)ay <(c)ash | (o)nline> [amount]\n\
        (s)top checkout\nGeneral commands:\n\
        (h)elp\n\
        (e)xit\n\
        (y)es, (n)o\n\
        "<<std::endl;
}