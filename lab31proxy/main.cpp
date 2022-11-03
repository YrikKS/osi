#include <iostream>
#include "Client/ClientsContainer.h"
#include "Socket/ServerSocketImpl.h"

int main() {
    ProxyServer::ServerSocketImpl serverSocket;
    serverSocket.connectSocket();
    return 0;
}
