#include <iostream>
#include "server/NewServerImpl.h"
#include "logger/Logger.h"
#include <signal.h>

//#include "cash/CashImpl.h"
//#include "buffer/BufferImpl.h"

#include <pthread.h>
int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    signal(SIGPIPE, SIG_IGN);
    try {
        ProxyServer::NewServerImpl *server = new ProxyServer::NewServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    LOG_EVENT("close app");
//    LOGGER_CLOSE;
    return 0;
}
