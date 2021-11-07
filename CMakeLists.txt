cmake_minimum_required(VERSION 3.20)
project(List)

set(CMAKE_CXX_STANDARD 14)
include_directories(includes src ./)

add_executable(List main.cpp src/list.cpp src/list_test.cpp)
