//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

void ProxyServer::ClientImpl::sendResponse() {

}

void ProxyServer::ClientImpl::getFdClient() {

}

void ProxyServer::ClientImpl::sendBuf(char *buf) {

}

char *ProxyServer::ClientImpl::readBuf() {
    return nullptr;
}

ProxyServer::ClientImpl::ClientImpl(int sock) {
    fd = sock;
}
