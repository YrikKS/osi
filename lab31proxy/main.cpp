#include <iostream>
#include "ServerImpl.h"
#include "logger/Logger.h"

int main() {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl* server = new ProxyServer::ServerImpl();
        delete server;
//        server->startServer();
    } catch (std::exception& ex) {
        ex.what();
        std::cout.flush();
        std::cout << "stop1" << std::endl;
        std::cout.flush();
        LOG_ERROR("close app by exception");
        std::cout << "clea4" << std::endl;
//        delete server;
        LOGGER_CLOSE;
        return 1;
    }
//    delete server;
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
