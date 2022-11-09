//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"
using namespace ProxyServer;

int ClientImpl::getFdClient() {
    return _fd;
}

void ClientImpl::sendBuf(const char *buf) {
    write(_fd, buf, std::strlen(buf));
}

int ClientImpl::readBuf(char *buf) {
    int byte = read(_fd, buf, BUF_SIZE - 1);
    std::cout << strlen(buf) << " ws " << byte << std::endl;
    std::cout.flush();
    return byte;
}

ClientImpl::ClientImpl(int sock, TypeClient typeClient, Buffer* buf) {
    _fd = sock;
    _typeClient = typeClient;
    _buffer = buf;
    _structPollFd.fd = _fd;
    _structPollFd.events = POLLIN | POLLOUT;
    _structPollFd.revents = 0;
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

struct pollfd ClientImpl::getPollFd() {
    return _structPollFd;
}

void ClientImpl::setPollElement(struct pollfd pollfd) {
    _structPollFd.fd = pollfd.fd;
    _structPollFd.events = pollfd.events;
    _structPollFd.revents = pollfd.revents;
}

void ClientImpl::setReventsZero() {
    _structPollFd.revents = 0;
}
