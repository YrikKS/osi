//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

void ProxyServer::ClientImpl::sendResponse() {

}

int ProxyServer::ClientImpl::getFdClient() {
    return fd;
}

void ProxyServer::ClientImpl::sendBuf(char *buf) {

}

char *ProxyServer::ClientImpl::readBuf() {
    return nullptr;
}

ProxyServer::ClientImpl::ClientImpl(int sock) {
    fd = sock;
}
