cmake_minimum_required(VERSION 3.5)

project(ppearls)

add_executable(ppearls main.cpp)
set_target_properties(ppearls PROPERTIES CXX_STANDARD 17)
target_include_directories(ppearls PUBLIC include)

add_executable(column1_3 column_1/3.cpp)
set_target_properties(column1_3 PROPERTIES CXX_STANDARD 17)
