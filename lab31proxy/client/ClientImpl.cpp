//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

int ProxyServer::ClientImpl::getFdClient() {
    return fd;
}

void ProxyServer::ClientImpl::sendBuf(const char *buf) {
    write(fd, buf, BUF_SIZE);
}

int ProxyServer::ClientImpl::readBuf(char *buf) {
    int byte = read(fd, buf, BUF_SIZE);
//    std::cout << buf << std::endl;
    return byte;
}

ProxyServer::ClientImpl::ClientImpl(int sock, STATUS_REQUEST statusRequest) {
    fd = sock;
    clientData = new ClientDataImpl(statusRequest);
}

ProxyServer::ClientImpl::~ClientImpl() {
    LOG_EVENT("destructor client");
    close(fd);
    delete clientData;
}

ProxyServer::ClientData* ProxyServer::ClientImpl::getClientData() {
    return clientData;
}
