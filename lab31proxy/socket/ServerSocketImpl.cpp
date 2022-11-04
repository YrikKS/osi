//
// Created by kurya on 03.11.2022.
//

#include <iostream>
#include "ServerSocketImpl.h"

int ProxyServer::ServerSocketImpl::connectSocket() {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        LOG_ERROR("create socket");
        return 1;
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(PORT_SERVER_SOCKET);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_family = AF_INET;

    if (bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        LOG_ERROR("bind socket");
        return 2;
    }

    if (listen(sockFd, 3) <
        0) {                                                                                                                                                 // выразить готовность принимать входящие соединения и задать размер очереди
        perror("listen");
        LOG_ERROR("listen socket");
        return 3;
    }
    serverSocket_ = sockFd;
    return 0;
}

int ProxyServer::ServerSocketImpl::getFdSocket() {
    return serverSocket_;
}

ProxyServer::Client *ProxyServer::ServerSocketImpl::acceptNewClient() {
    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;
    if ((clientSock = accept(serverSocket_, (struct sockaddr *) &clientAddr, &len)) <
        0) {                                                                                                         // вытаскивает из очереди 1 элемент и устанавливает соединение
        perror("accept");
        LOG_ERROR("accept new client");
        return NULL; // TODO exeption
    }

    Client *client = new ClientImpl(clientSock);
    LOG_EVENT("accept new client");
    return client;
}

void ProxyServer::ServerSocketImpl::closeSocket() {
    LOG_EVENT("close server socket");
    close(serverSocket_);
}

ProxyServer::ServerSocketImpl::~ServerSocketImpl() {
    close(serverSocket_);
}
