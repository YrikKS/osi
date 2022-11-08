#include <iostream>
#include "server/Server.h"
#include "server/ServerImpl.h"
#include "logger/Logger.h"

int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    try {
        ProxyServer::ServerImpl *server = new ProxyServer::ServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
        std::cout << "clea4" << std::endl;
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
