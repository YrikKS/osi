#include <iostream>
#include "server/NewServerImpl.h"
#include <memory>
#include <signal.h>

int main(int argc, char *argv[]) {
    LOG_EVENT("start program");
    signal(SIGPIPE, SIG_IGN);
    try {
        auto server = std::unique_ptr<ProxyServer::Server>(new ProxyServer::NewServerImpl());
        server->startServer();
    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
        LOG_ERROR("close app by exception");
    }
    LOG_EVENT("close app");
    return 0;
}
