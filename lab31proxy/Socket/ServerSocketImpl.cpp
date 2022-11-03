//
// Created by kurya on 03.11.2022.
//

#include <iostream>
#include "ServerSocketImpl.h"

int ProxyServer::ServerSocketImpl::connectSocket() {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(PORT_SERVER_SOCKET);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_family = AF_INET;

    if (bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        return 2;
    }
    std::cout << "listen" << std::endl;
    if (listen(sockFd, 3) <
        0) {                                                                                                                                                 // выразить готовность принимать входящие соединения и задать размер очереди
        perror("listen");
        return 3;
    }
    proxyServerSocket_ = sockFd;
}

int ProxyServer::ServerSocketImpl::getFdSocket() {
    return proxyServerSocket_;
}

ProxyServer::Client *ProxyServer::ServerSocketImpl::acceptNewClient() {
    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;
    if ((clientSock = accept(proxyServerSocket_, (struct sockaddr *) &clientAddr, &len)) <
        0) {                                                                                                         // вытаскивает из очереди 1 элемент и устанавливает соединение
        perror("accept");
        return NULL; // TODO exeption
    }
    Client *client = new ClientImpl(clientSock);
    return client;
}

void ProxyServer::ServerSocketImpl::closeSocket() {
    close(proxyServerSocket_);
}
