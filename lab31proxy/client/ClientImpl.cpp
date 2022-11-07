//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"
using namespace ProxyServer;

int ClientImpl::getFdClient() {
    return _fd;
}

void ClientImpl::sendBuf(const char *buf) {
    std::cout << "send : " << buf << std::endl;
    write(_fd, buf, std::strlen(buf));
}

int ClientImpl::readBuf(char *buf) {
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

ClientImpl::ClientImpl(int sock, TypeClient typeClient, Buffer* buf) {
    _fd = sock;
    _typeClient = typeClient;
    _buffer = buf;
}

ClientImpl::~ClientImpl() {
    LOG_EVENT("destructor client");
    close(_fd);
}

TypeClient ClientImpl::getTypeClient() {
    return _typeClient;
}

Buffer *ClientImpl::getBuffer() {
    return _buffer;
}

void ClientImpl::setBuffer(Buffer *buffer) {
    _buffer = buffer;
}

Client *ClientImpl::getPair() {
    return _pair;
}

void ClientImpl::setPair(Client *pair) {
    _pair = pair;
}

struct pollfd *ClientImpl::getPollFd() {
    return _structPollFd;
}

void ClientImpl::setPollfd(struct pollFd *pollfd) {
    _structPollFd = pollfd;
}
