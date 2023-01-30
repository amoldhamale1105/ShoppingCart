#include <iostream>
#include <Inventory.h>
#include <String.hpp>
#include <Pair.hpp>
#include <EventLoop.hpp>

String input, userName;
Inventory inventory;

void getUserInput(Event*);
void cleanup(Event*);
int findProduct(const String&);
void usage();

int main(int argc, char** argv)
{
    userName = argc > 1 ? argv[1] : "Customer";
    
    std::cout<<"---Welcome to Small Basket shopping cart shell---"<<std::endl;
    usage();

    EventLoop::RegisterEvents({"IShop","IPay"}, getUserInput);
    EventLoop::RegisterEvent("CleanMem", cleanup);
    EventLoop::TriggerEvent("IShop");

	EventLoop::Run();

    return 0;
}

void getUserInput(Event *evt)
{
    std::cout<<userName<<"@SmallBasket:$ ";
    std::cin >> input;
    input.trim();
    if (input.empty()){
        EventLoop::TriggerEvent("IShop");
        return;
    }
    std::string evtName = evt->getName();

    if (evtName == "IShop"){
        Vector<String> commandVec = input.split(' ');
        if (!commandVec.isEmpty()){
            String command = commandVec.front();
            if (command == "a" || command == "add"){
                if (commandVec.size() < 3){
                    std::cout<<"Incomplete command. Enter (h)elp for usage details"<<std::endl;
                    EventLoop::TriggerEvent("IShop");
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
                if (productIndex == -1 || productIndex >= inventory.database.size()){
                    std::cout<<"Invalid product name/index. Given product does not exist"<<std::endl;
                    EventLoop::TriggerEvent("IShop");
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
                    EventLoop::TriggerEvent("IShop");
                    return;
                }

                EventLoop::TriggerEvent("AddItem", new Pair<Product*,int>(inventory.database[productIndex], std::stoi(quantityArg.c_str())));
            }
            else if (command == "h" || command == "help"){
                usage();
                EventLoop::TriggerEvent("IShop");
                return;
            }
            else{
                std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("IShop");
                return;
            }
        }
        else{
            if (input == "h" || input == "help"){
                usage();
                EventLoop::TriggerEvent("IShop");
                return;
            }
            else{
                std::cout<<"Invalid command. Enter (h)elp for usage details"<<std::endl;
                EventLoop::TriggerEvent("IShop");
                return;
            }
        }
    }
    else if (evtName == "IPay"){

    }
}

void cleanup(Event *evt)
{
    if (evt->getData() != nullptr)
        free(evt->getData());
}

int findProduct(const String &name)
{
    int targetIndex{-1};
    int totalItems = inventory.database.size();
    for(auto i = 0; i < totalItems; i++)
    {
        if (strcasecmp(inventory.database.at(i)->getName().c_str(), name.c_str()) == 0){
            targetIndex = i;
            break;
        }
    }
    return targetIndex;
}

void usage()
{
    std::cout<<"Commands to shop:\n\
                (a)dd <category-index | product-name> <quantity>\n\
                (r)emove <category-index | product-name> <quantity>\n\
                (v)iew cart\n\
                (c)heckout\n\
                (y)es, (n)o\n\
                Special commands:\n\
                (h)elp\n\
                (s)top payment\n"<<std::endl;
}
