//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

int ProxyServer::ClientImpl::getFdClient() {
    return _fd;
}

void ProxyServer::ClientImpl::sendBuf(const char *buf) {
    std::cout << "send : " << buf << std::endl;
    write(_fd, buf, std::strlen(buf));
}

int ProxyServer::ClientImpl::readBuf(char *buf) {
    int byte = read(_fd, buf, BUF_SIZE - 1);
//    std::cout << buf << std::endl << std::endl;
//    std::string str = buf;
//    for(int i = 0; i < str.size(); i++) {
//        int a = static_cast<int>(str[i]);
//        std::cout << str[i] << " = " << a << " ";
//        std::cout.flush();
//    }
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

struct pollfd *ProxyServer::ClientImpl::getPollFd() {
    return _structPollFd;
}

void ProxyServer::ClientImpl::setPollfd(struct pollFd *pollfd) {
    _structPollFd = pollfd;
}
