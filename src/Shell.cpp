#include "Shell.hpp"

Shell::Shell(const String &user) : m_userName(user), m_exitFlag(false), m_paymentFlag(false)
{
    EventLoop::RegisterEvents({"Shop","Pay"}, std::bind(&Shell::getUserInput, this, std::placeholders::_1));
    EventLoop::RegisterEvent("CleanMem", std::bind(&Shell::cleanup, this, std::placeholders::_1));

    std::cout<<"---Welcome to Small Basket shopping cart shell---"<<std::endl;
    usage();
}

Shell::~Shell()
{
}

void Shell::getUserInput(Event *evt)
{
    std::cout<<m_userName<<"@SmallBasket:$ ";
    std::cin >> m_input;
    m_input.trim();
    if (m_input.empty()){
        EventLoop::TriggerEvent("Shop");
        return;
    }
    std::string evtName = evt->getName();

    if (evtName == "Shop"){
        Vector<String> commandVec = m_input.split(' ');
        if (!commandVec.isEmpty()){
            String command = commandVec.front();
            if (command == "a" || command == "add"){
                if (m_exitFlag)
                    m_exitFlag = false;
                if (commandVec.size() < 3){
                    std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                    EventLoop::TriggerEvent("Shop");
                    return;
                }
                String productArg = commandVec.at(1);
                Product* product = nullptr;
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

                String quantityArg = commandVec.at(2);
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
            else if (command == "h" || command == "help"){
                usage();
                EventLoop::TriggerEvent("Shop");
                return;
            }
            else if (command == "e" || command == "exit"){
                std::cout<<"Are you sure? Your current session and cart status will be lost"<<std::endl;
                m_exitFlag = true;
                EventLoop::TriggerEvent("Shop");
                return;
            }
            else if (command == "y" || command == "yes"){
                if (m_exitFlag){
                    std::cout<<"Thank you for shopping with us. See you again!"<<std::endl;
                    EventLoop::Halt();
                    return;
                }
                std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
            else if (command == "n" || command == "no"){
                if (m_exitFlag){
                    std::cout<<"Good call! Please continue shopping and enter (h)elp if needed"<<std::endl;
                    m_exitFlag = false;
                }
                else
                    std::cout<<"Unexpected command in current context. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;                
            }
            else{
                std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
        }
        else{
            if (m_input == "h" || m_input == "help"){
                usage();
                EventLoop::TriggerEvent("Shop");
                return;
            }
            else{
                std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("Shop");
                return;
            }
        }
    }
    else if (evtName == "Pay"){

    }
}

void Shell::cleanup(Event *evt)
{
    if (evt->getData() != nullptr)
        free(evt->getData());
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
    std::cout<<"Commands to shop:\n\
        (a)dd <category-index | product-name> <quantity>\n\
        (r)emove <category-index | product-name> <quantity>\n\
        (v)iew cart\n\
        (c)heckout\nOther commands:\n\
        (h)elp\n\
        (e)xit\n\
        (y)es, (n)o\n\
        (s)top payment\n"<<std::endl;
}