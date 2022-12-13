//
// Created by kurya on 03.11.2022.
//

#include <iostream>
#include "ServerSocketImpl.h"


using namespace ProxyServer;

int ServerSocketImpl::connectSocket() {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR_WITH_ERRNO("create socket");
        throw ConnectException("bind socket");
    }

    const int enable = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        LOG_ERROR_WITH_ERRNO("setsockopt(SO_REUSEADDR) failed");
        throw ConnectException("setsockopt(SO_REUSEADDR) failed");
    }
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        LOG_ERROR_WITH_ERRNO("setsockopt(SO_REUSEADDR) failed");
        throw ConnectException("setsockopt(SO_REUSEADDR) failed");
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

int ServerSocketImpl::getFdSocket() {
    return serverSocket_;
}

Client *ServerSocketImpl::acceptNewClient(Cash *cash) {
    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;
    if ((clientSock = accept(serverSocket_, (struct sockaddr *) &clientAddr, &len)) < 0) {
        LOG_ERROR_WITH_ERRNO("error accept new client");
        throw ConnectException("error accept new client");
    }

    Client *client = new ClientImpl(clientSock, TypeClient::USER, new BufferImpl(cash));
    client->getBuffer()->setIsClientConnect(true);
    LOG_EVENT("accept new client");
    return client;
}


int ServerSocketImpl::acceptNewClientSock() {
    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;
    if ((clientSock = accept(serverSocket_, (struct sockaddr *) &clientAddr, &len)) < 0) {
        LOG_ERROR_WITH_ERRNO("error accept new client");
        throw ConnectException("error accept new client");
    }
    LOG_EVENT("accept new client with out problem");
    return clientSock;
}

void ServerSocketImpl::closeSocket() {
    LOG_EVENT("close server socket");
    close(serverSocket_);
}

ServerSocketImpl::~ServerSocketImpl() {
    close(serverSocket_);
}

Client *ServerSocketImpl::connectToClient(std::string url, int port) {
    pthread_mutex_lock(&mutexForServer);
    struct hostent *hostent = gethostbyname(url.data());
    if (hostent == NULL) {
        LOG_ERROR("gethostbyname");
        herror("gethostbyname");
        pthread_mutex_unlock(&mutexForServer);
        throw ConnectException("gethostbyname");
    }
    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
    sockAddr.sin_port = htons(port);
    sockAddr.sin_family = AF_INET;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == ERROR_CODE) {
        LOG_ERROR_WITH_ERRNO("setsockopt");
        pthread_mutex_unlock(&mutexForServer);
        throw ConnectException("setsockport");
    }
    if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == ERROR_CODE) {
        perror("connect: ");
        LOG_ERROR_WITH_ERRNO("connect: ");
        pthread_mutex_unlock(&mutexForServer);
        throw ConnectException("connect error");
    }
    LOG_EVENT("http server connect");
    Client *client = new ClientImpl(sock, TypeClient::HTTP_SERVER, NULL);
    pthread_mutex_unlock(&mutexForServer);
    return client;
}

ServerSocketImpl::ServerSocketImpl() {

}

