# Shopping Cart
This app is a shopping cart shell for a hypothetical e-commerce platform named `Small Basket`  
It is developed using custom built data structures (https://github.com/amoldhamale1105/CustomDataStructures) and event loop (https://github.com/amoldhamale1105/EventLoop)     
The shopping cart shell accepts and executes commands for various functions of the shopping application  

## Build instructions
The app can be built using the `build.sh` script in the project source tree. It can be run without any options for default build configuration. Defaults for each option will appear in `{}` in the usage instruction. Learn about script usage with the following command
```
./build.sh -h
```
**Note:** The following steps to build are not ideal since the app does not depend on all the headers of *EventLoop* and *CustomDataStructures* projects. A better way to structure the build system would be to download the required headers and libraries in any location without having to clone and build the entire projects. This change will be introduced and readme updated after the next release of `EventLoop` library  

The app depends on external libraries and headers. The build script is equipped to accept the path to those dependencies in order to build the project. The following steps can be followed to resolve dependencies
- Clone or download and unzip https://github.com/amoldhamale1105/CustomDataStructures and https://github.com/amoldhamale1105/EventLoop
- Follow the build instructions under each project readme to generate dependent artifacts
- Run the build script of this project with the `-p` option and argument as comma separated list of paths (absolute or relative) to dependencies. For example, if you clone the above 2 projects at the same level as this project, the build script can be executed as follows with relative paths to the root of dependency projects:
```
./build.sh -p ../EventLoop/,../CustomDataStructures
```
Build artifacts will be generated in the `build` directory  
Output artifact will be present in the `bin` directory as `ShoppingCart` binary

## Supporting Documents
There are 2 documents used to run and test the shopping cart application
### Shopping catalogue => `Catalogue.docx`
This document contains the entire shopping catalogue segregated by categories like GROCERIES, ELECTRONICS, BOOKS etc.  
Each category has an index `(Category-Index)` and each item in the category has an index `(Item-Index)`, name and corresponding price  
The currency of the price is inconsequential in this context and thus can be assumed to be INR  
### Discount coupons => `DiscountCoupons.docx`
This document contains various discount coupons used by the app to offer user discounts  
Each discount coupon is associated with either an overall discount on total bill or category wise discount  
Only the discount coupons mentioned in the document will be considered valid  

## Output and Testing
Run the shopping cart app with the following command, optionally with a program argument of the customer name. The shell and cart details will use the customer name  
```
./bin/ShoppingCart Tony
```
The following shell will be commenced for the current customer. The shell will be preceeded with a manual on using the shell for shopping. This manual can be invoked with the following command in the cart shell
```
h
```
![image](https://user-images.githubusercontent.com/78597991/216771735-5bb5733a-59b8-4ed1-961c-fc11fea559c1.png)

As an example, a product can be added and removed from the cart as follows,  
Form a whole number using the category and item index, use it with the `add` or `a` command followed by quantity  
To start shopping, Tony might want to add 4 units of oats, 3 units of milk and 7 units of mango to his cart  
Oats `Item-Index = 3` lies in Groceries category `Category-Index = 1`. Thus `category-index` value of the add command will be `13`  
Milk `Item-Index = 1` lies in Groceries category `Category-Index = 4`. Thus `category-index` value of the add command will be `41`  
Mango `Item-Index = 4` lies in Groceries category `Category-Index = 2`. Thus `category-index` value of the add command will be `24`  
Tony will run commands in the cart shell as follows  
```
a 13 4
a 41 3
a 24 7
```
Let's say, Tony realized he added too many units of mango to cart and wishes to reduce the quantity by 2. Later he realises that he doesn't need milk at all because he already has a few litres left in his fridge  
Based on the index calculation above, Tony will run the following commands in the cart shell  
```
r 24 2
r 41
```
Tony decides to view the cart once before checking out to ensure everything's right  
```
v
```
He initiates checkout with the following command
```
c
```
Tony decided to pay part of the amount in cash and remaining through his `Small Basket` wallet. He runs the following commands to achieve the same  
```
p c 1000
p o
```
His shopping session is successful but not over. Now he can either continue shopping as before or exit the app with the following commands  
```
e
y
```
The above commands depict the short-hand version of the original commands. Short-hand enables quick shopping and checkout experience with less time spent in actually typing the full commands. Run the help command mentioned above to get the list of all commands

![image](https://user-images.githubusercontent.com/78597991/216771652-553531a2-5d24-4dcd-a9c0-a7870fd389fe.png)

## Contribution
This project is mainly a dogfooding ambition to prove utility of my custom built data structures and event loop library (links mentioned in the intro above). It also helped me to uncover a few bugs in those dependency projects. This project will also serve as an example for how to use my custom built data structures and event loop in your own projects and to give you an idea of how they work  
With that premise, you can review the code in this repository as well as from the data structures and event loop repo. I am open to improvements in the software as well as the architecture if it offers the expected functionality in a better way. I have tested the app extensively and also run leak checks with `valgrind`. However, often bugs slip through and unoptimized code gets overlooked. Therefore, I would like you to review, test and consequently raise pull requests or create issues if required.  
Please contact me in case of any questions or suggestions amoldhamale1105@gmail.com
