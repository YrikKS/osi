#include <iostream>
#include "Socket/ServerSocketImpl.h"

int main() {
    ProxyServer::ServerSocketImpl serverSocket;
    serverSocket.connectSocket();
    ProxyServer::Client* client = serverSocket.acceptNewClient();
    std::cout << "client fd == " << client->getFdClient() << std::endl;
    return 0;
}
