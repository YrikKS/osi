cmake_minimum_required(VERSION 3.0)
project(httpReauestSender)

set(CMAKE_CXX_STANDARD 23)

add_executable(httpReauestSender main.cpp)

add_executable(withUrl withUrl.cpp)

add_executable(server server.cpp)

add_executable(writer writer.cpp BinaryString.cpp)
