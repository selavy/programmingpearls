cmake_minimum_required(VERSION 3.5)

project(ppearls)

# All problems that I can do as unit tests:
add_executable(ppearls main.cpp)
set_target_properties(ppearls PROPERTIES CXX_STANDARD 17)
target_include_directories(ppearls PUBLIC include)

# Column #1:
add_executable(bitmap_sort
    column_1/bitmap_sort.cpp)
set_target_properties(bitmap_sort PROPERTIES CXX_STANDARD 17)

add_executable(bitmap_sort_low_memory
    column_1/bitmap_sort_low_memory.cpp)

add_executable(set_sort column_1/set_sort.cpp)
set_target_properties(set_sort PROPERTIES CXX_STANDARD 17)

add_executable(column1_gen_numbers
    column_1/gen_numbers.cpp)

add_executable(column1_bitmap_sort_duplicates
    column_1/bitmap_sort_duplicates.cpp)
set_target_properties(column1_bitmap_sort_duplicates PROPERTIES CXX_STANDARD 17)

# Column #2:
add_executable(column2_rotate
    column_2/rotate.cpp)
set_target_properties(column2_rotate PROPERTIES CXX_STANDARD 17)
