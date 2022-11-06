//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

int ProxyServer::ClientImpl::getFdClient() {
    return _fd;
}

void ProxyServer::ClientImpl::sendBuf(const char *buf) {
    std::cout << "send : " << buf << std::endl;
    write(_fd, buf, BUF_SIZE);
}

int ProxyServer::ClientImpl::readBuf(char *buf) {
    int byte = read(_fd, buf, BUF_SIZE);
//    std::cout << buf << std::endl;
    std::string str = buf;
    for(int i = 0; i < str.size(); i++) {
        std::cout << (int)str[i] << " ";
    }
    return byte;
}

ProxyServer::ClientImpl::ClientImpl(int sock, TypeClient typeClient, Buffer* buf) {
    _fd = sock;
    _typeClient = typeClient;
    _buffer = buf;
}

ProxyServer::ClientImpl::~ClientImpl() {
    LOG_EVENT("destructor client");
    close(_fd);
}

ProxyServer::TypeClient ProxyServer::ClientImpl::getTypeClient() {
    return _typeClient;
}

ProxyServer::Buffer *ProxyServer::ClientImpl::getBuffer() {
    return _buffer;
}

void ProxyServer::ClientImpl::setBuffer(ProxyServer::Buffer *buffer) {
    _buffer = buffer;
}

ProxyServer::Client *ProxyServer::ClientImpl::getPair() {
    return _pair;
}

void ProxyServer::ClientImpl::setPair(ProxyServer::Client *pair) {
    _pair = pair;
}
