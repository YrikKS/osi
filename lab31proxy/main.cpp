#include <iostream>
#include "ServerImpl.h"
#include "logger/Logger.h"

int main() {
    ProxyServer::ServerImpl server = ProxyServer::ServerImpl();
    server.startServer();
    LOGGER_CLOSE;
    return 0;
}
