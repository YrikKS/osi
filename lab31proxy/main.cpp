#include <iostream>
#include "ServerImpl.h"
#include "logger/Logger.h"

int main() {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl server = ProxyServer::ServerImpl();
        server.startServer();
    } catch (ProxyServer::ConnectException *ex) {
        std::cerr << ex->what() << std::endl;
        LOG_ERROR("close app by exception");
        LOGGER_CLOSE;
        return 1;
    }

    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
