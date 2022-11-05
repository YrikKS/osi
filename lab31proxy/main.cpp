#include <iostream>
#include "server/ServerImpl.h"
#include "logger/Logger.h"

int main() {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl* server = new ProxyServer::ServerImpl();
        std::cout << server << std::endl;
//        delete server;
        server->startServer();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
        std::cout << "clea4" << std::endl;
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
