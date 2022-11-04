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
//    std::cout << buf << std::endl;
    return byte;
}

ProxyServer::ClientImpl::ClientImpl(int sock, STATUS_REQUEST statusRequest) {
    fd = sock;
    this->statusRequest = statusRequest;
}

ProxyServer::ClientImpl::~ClientImpl() {
    close(fd);
}

ProxyServer::STATUS_REQUEST ProxyServer::ClientImpl::getStatusRequest() {
    return statusRequest;
}

void ProxyServer::ClientImpl::setStatusRequest(ProxyServer::STATUS_REQUEST statusRequest) {
    ClientImpl::statusRequest = statusRequest;
}

void ProxyServer::ClientImpl::setRequestHeading(const std::string &requestHeading) {
    ClientImpl::requestHeading = requestHeading;
}

std::string &ProxyServer::ClientImpl::getRequestHeading() {
    return requestHeading;
}
