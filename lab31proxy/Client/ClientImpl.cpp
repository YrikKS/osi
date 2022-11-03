//
// Created by kurya on 03.11.2022.
//

#include <cstring>
#include <unistd.h>
#include <iostream>
#include "ClientImpl.h"

void ProxyServer::ClientImpl::sendResponse() {

}

int ProxyServer::ClientImpl::getFdClient() {
    return fd;
}

void ProxyServer::ClientImpl::sendBuf(char *buf) {

}

char *ProxyServer::ClientImpl::readBuf() {
    char buf[1024] = {0};
    std::cout << read(fd, buf, 1024 - 1) << std::endl;
    std::cout << buf << std::endl;
    return NULL;
}

ProxyServer::ClientImpl::ClientImpl(int sock) {
    fd = sock;
}

ProxyServer::ClientImpl::~ClientImpl() {
    close(fd);
}
