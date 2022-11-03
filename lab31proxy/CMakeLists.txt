cmake_minimum_required(VERSION 3.0)
project(lab31proxy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(testSocket
        main.cpp
        Socket/ServerSocketImpl.cpp
        Client/ClientImpl.cpp
        Client/Client.h
        )
#add_executable(lab31proxy main.cpp
#        Server.h
#        ServerImpl.cpp
#        Constants.h
#        Client/Client.h
#        Client/ClientImpl.cpp
#        Socket/ServerSocket.h
#        Socket/ServerSocketImpl.cpp Socket/ServerSocketImpl.h
#        )
