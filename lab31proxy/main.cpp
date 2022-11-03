#include <iostream>
#include "ServerImpl.h"


int main() {
    ProxyServer::ServerImpl server = ProxyServer::ServerImpl();
    server.startServer();
    return 0;
}
