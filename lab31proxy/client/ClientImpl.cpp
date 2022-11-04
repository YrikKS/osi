//
// Created by kurya on 03.11.2022.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "ClientImpl.h"
#include "../Constants.h"

int ProxyServer::ClientImpl::getFdClient() {
    return fd;
}

void ProxyServer::ClientImpl::sendBuf(char *buf) {
    write(fd, buf, BUF_SIZE);
}

int ProxyServer::ClientImpl::readBuf(char *buf) {
    int byte = read(fd, buf, BUF_SIZE);
    std::cout << buf << std::endl;
    return byte;
}

ProxyServer::ClientImpl::ClientImpl(int sock) {
    fd = sock;
}

ProxyServer::ClientImpl::~ClientImpl() {
    close(fd);
}
