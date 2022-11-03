#include <iostream>
#include "Socket/ServerSocketImpl.h"

int main() {
    ProxyServer::ServerSocketImpl serverSocket;
    serverSocket.connectSocket();
    return 0;
}
