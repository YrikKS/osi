cmake_minimum_required(VERSION 3.0)
project(lab23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(lab23 main.cpp PthreadSaveList.h)
