//
// Created by kurya on 03.11.2022.
//

#include <iostream>
#include "ServerSocketImpl.h"

int ProxyServer::ServerSocketImpl::connectSocket() {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR_WITH_ERRNO("create socket");
        throw ConnectException("bind socket");
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(PORT_SERVER_SOCKET);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_family = AF_INET;

    if (bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) < 0) {
        LOG_ERROR_WITH_ERRNO("bind socket");
        throw ConnectException("bind socket");
    }

    if (listen(sockFd, 3) < 0) {
        LOG_ERROR_WITH_ERRNO("listen socket");
        throw ConnectException("listen socket");
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
    if ((clientSock = accept(serverSocket_, (struct sockaddr *) &clientAddr, &len)) < 0) {
        LOG_ERROR_WITH_ERRNO("accept new client");
        throw ConnectException("accept new client");
    }

    Client *client = new ClientImpl(clientSock, StatusHttp::WRITE_REQUEST_HEADING,
                                    TypeClient::USER, new BufferImpl());
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

ProxyServer::Client *ProxyServer::ServerSocketImpl::connectToClient(std::string url, int port) {
//    for(int i =0; i < url.size(); i++) {
//        std::cout << "i = " << (int) url.c_str()[i] << std::endl;
//    }
//    std::cout.flush();
    struct hostent *hostent = gethostbyname(url.data());
    if (hostent == NULL) {
        LOG_ERROR("gethostbyname");
        herror("gethostbyname");
        throw ConnectException("gethostbyname");
    }

    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
    sockAddr.sin_port = htons(port);
    sockAddr.sin_family = AF_INET;

    int sock = socket(AF_INET, SOCK_STREAM, 0); //PF_INET
    if (sock == ERROR_CODE) {
        LOG_ERROR_WITH_ERRNO("setsockopt");
        throw ConnectException("setsockport");
    }
    if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == ERROR_CODE) {
        LOG_ERROR_WITH_ERRNO("connect");
        throw ConnectException("setsockport");
    }

    LOG_EVENT("http server connect");
    Client *client = new ClientImpl(sock, StatusHttp::READ_REQUEST, TypeClient::HTTP_SERVER, NULL);
    std::cout << "client connect!" << std::endl;
    return client;
}
