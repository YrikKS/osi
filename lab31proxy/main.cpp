#include <iostream>
#include "server/NewServerImpl.h"
#include "logger/Logger.h"

//#include "cash/CashImpl.h"
//#include "buffer/BufferImpl.h"

int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    try {
        ProxyServer::NewServerImpl *server = new ProxyServer::NewServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    LOG_EVENT("close app");
    LOGGER_CLOSE;
    return 0;
}
