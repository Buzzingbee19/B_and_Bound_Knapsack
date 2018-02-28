cmake_minimum_required(VERSION 3.7)
project (4542_Project_4a)

set(CMAKE_CXX_STANDARD 11)

set(ADDED_FILES d_except.h d_random.h d_matrix.h)
set(SOURCE_FILES main.cpp knapsack.h ${ADDED_FILES} knapsack.cpp)
add_executable(main ${SOURCE_FILES})
