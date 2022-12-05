//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

using namespace ProxyServer;

int ClientImpl::getFdClient() {
    return _fd;
}

void ClientImpl::sendBuf(std::string *buf) {
    write(_fd, (buf)->c_str(), (buf)->length());
}

void ClientImpl::readBuf(std::string *buf) { // передать пустой буффер размером BUF_SIZE!
    char readBuf[BUF_SIZE] = {0};
    int byte = read(_fd, readBuf, BUF_SIZE - 1);
    if (byte < 0) {
        perror("read error");
        buf->clear();
        return;
    }
//    (buf)->clear();
    (buf)->resize(byte, ' ');
    std::memcpy((void *) (buf)->c_str(), readBuf, byte);
}

ClientImpl::ClientImpl(int sock, TypeClient typeClient, Buffer *buf) {
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

void ClientImpl::setEvents(int event) {
    _structPollFd.events = event;
}
