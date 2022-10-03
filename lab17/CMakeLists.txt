cmake_minimum_required(VERSION 3.0)
project(lab17)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(lab17 main.cpp PthreadSaveList.h)
