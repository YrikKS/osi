#include <iostream>
#include "server/NewServerImpl.h"
#include <signal.h>

int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    signal(SIGPIPE, SIG_IGN);
    ProxyServer::Server *server = nullptr;
    try {
        server = new ProxyServer::NewServerImpl();
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    delete server;
    LOG_EVENT("close app");
    return 0;
}
