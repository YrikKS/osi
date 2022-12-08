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

std::list<Client *> ClientImpl::getListHandlingEvent() {
    return _listHandlingEvent;
}

void ClientImpl::eraseIt(Client *client) {
    for (auto it = _listHandlingEvent.begin(); it != _listHandlingEvent.end(); it++) {
        if (client == *it) {
            _listHandlingEvent.erase(it);
            break;
        }
    }
}

void ClientImpl::addClientToHandlingEvent(Client *client) {
    _listHandlingEvent.push_back(client);
}
