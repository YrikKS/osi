cmake_minimum_required(VERSION 3.24)
project(lab18)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(lab18 main.cpp PthreadSaveList.h)
