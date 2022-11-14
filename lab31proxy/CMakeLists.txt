cmake_minimum_required(VERSION 3.0)
project(lab31proxy)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS -std=c++11)
#add_executable(test
#        main.cpp
#        Constants.h
#        parser/ParserTypeRequest.h
#        parser/ParserImpl.cpp
#        parser/ParserImpl.h
#        logger/Logger.h
##        socket/ConnectException.h
#        parser/ResultParseHeading.h
#        parser/ParseException.h
##        buffer/Buffer.h
##        buffer/BufferImpl.cpp
##        buffer/BufferImpl.h
#        cash/Cash.h
#        cash/CashElement.h
#        cash/CashElementImpl.cpp
#        cash/CashElementImpl.h
#        cash/CashImpl.cpp
#        cash/CashImpl.h
#        binaryString/BinaryString.cpp
#        binaryString/BinaryString.h
#        )

add_executable(lab31proxy
        main.cpp
        server/Server.h
        server/ServerImpl.cpp
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
#        client/ClientData.h
#        client/ClientDataImpl.cpp
#        client/ClientDataImpl.h
        buffer/Buffer.h
        buffer/BufferImpl.cpp
        buffer/BufferImpl.h
        cash/Cash.h
        cash/CashElement.h
        cash/CashElementImpl.cpp
        cash/CashElementImpl.h
        cash/CashImpl.cpp
        cash/CashImpl.h
        binaryString/BinaryString.cpp
        binaryString/BinaryString.h
        )
#include_directories(${Boost_INCLUDE_DIRS})

#target_link_libraries(lab31proxy
#        boost/version.hpp
#        ${BOOST_ROOT}/libs/regex/src/regex.cpp
#        ${BOOST_ROOT}/libs/regex/src/static_mutex.cpp
#        ${BOOST_ROOT}/libs/thread/src/win32/thread.cpp
#        )
