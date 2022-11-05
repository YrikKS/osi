#include <iostream>
#include "ServerImpl.h"
#include "logger/Logger.h"

int main() {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl server = ProxyServer::ServerImpl();
        server.startServer();
    } catch (std::exception& ex) {
        std::cout.flush();
        std::cout << "stop1" << std::endl;
        std::cout.flush();
        LOG_ERROR("close app by exception");
        std::cout << "clea4" << std::endl;
        LOGGER_CLOSE;
        ex.what();
        return 1;
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
