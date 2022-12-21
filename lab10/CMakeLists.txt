cmake_minimum_required(VERSION 3.0)
project(lab10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread -std=c++11")
add_executable(lab10 main.cpp)
