#include <iostream>
#include "ServerImpl.h"
#include "logger/Logger.h"

int main() {
    LOG_EVENT("start program");
    ProxyServer::ServerImpl server = ProxyServer::ServerImpl();
    server.startServer();
    LOGGER_CLOSE;
    return 0;
}
