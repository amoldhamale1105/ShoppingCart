cmake_minimum_required(VERSION 3.16)

project(ShoppingCart VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(EVENT_LOOP_LIB EventLoop)
find_library(CUSTOM_DS_LIB CustomDataStructures)

find_path(CUSTOM_DS_INCLUDES Vector.hpp)
find_path(EVENT_LOOP_INCLUDES EventLoop.hpp)

include_directories(include)

set(SOURCES
    src/Product.cpp
    src/Item.cpp
    src/Cart.cpp
    src/Inventory.cpp
    src/Shell.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME} 
    ${SOURCES}
    main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${CUSTOM_DS_LIB}
    ${EVENT_LOOP_LIB})
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CUSTOM_DS_INCLUDES}
    ${EVENT_LOOP_INCLUDES})