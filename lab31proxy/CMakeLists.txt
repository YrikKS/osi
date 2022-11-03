cmake_minimum_required(VERSION 3.0)
project(lab31proxy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS -std=c++11)
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#FIND_PACKAGE(Boost)
#add_executable(testSocket
#        main.cpp
#        socket/ServerSocketImpl.cpp
#        client/ClientImpl.cpp
#        client/client.h
#        )
add_executable(lab31proxy
        main.cpp
        Server.h
        ServerImpl.cpp
        Constants.h
        client/Client.h
        client/ClientImpl.cpp
        socket/ServerSocket.h
        socket/ServerSocketImpl.cpp
        socket/ServerSocketImpl.h
        )
#include_directories(${Boost_INCLUDE_DIRS})

#target_link_libraries(lab31proxy
#        boost/version.hpp
#        ${BOOST_ROOT}/libs/regex/src/regex.cpp
#        ${BOOST_ROOT}/libs/regex/src/static_mutex.cpp
#        ${BOOST_ROOT}/libs/thread/src/win32/thread.cpp
#        )
