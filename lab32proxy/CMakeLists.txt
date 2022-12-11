cmake_minimum_required(VERSION 3.0)
project(lab32proxy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-pthread -std=c++11")
#set(CMAKE_CXX_FLAGS -std=c++11)

add_executable(lab32proxy
        main.cpp
        server/Server.h
        Constants.h
        client/Client.h
        client/ClientImpl.cpp
        socket/ServerSocket.h
        socket/ServerSocketImpl.cpp
        socket/ServerSocketImpl.h
        parser/ParserTypeRequest.h
        parser/ParserImpl.cpp
        parser/ParserImpl.h
        logger/Logger.h
        socket/ConnectException.h
        parser/ResultParseHeading.h
        parser/ParseException.h
        buffer/Buffer.h
        buffer/BufferImpl.cpp
        buffer/BufferImpl.h
        cash/Cash.h
        cash/CashElement.h
        cash/CashElementImpl.cpp
        cash/CashElementImpl.h
        cash/CashImpl.cpp
        cash/CashImpl.h
        server/NewServerImpl.cpp
        server/NewServerImpl.h
        server/HandlerOneClientImpl.cpp server/HandlerOneClientImpl.h server/HandlerOneClient.h server/ArgsForThread.h)