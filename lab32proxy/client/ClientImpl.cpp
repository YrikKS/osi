//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

using namespace ProxyServer;

int ClientImpl::getFdClient() {
    return _fd;
}

int ClientImpl::sendBuf(std::string *buf) {
    int code = write(_fd, (buf)->c_str(), (buf)->length());
    return code;
}

int ClientImpl::readBuf(std::string *buf) { // передать пустой буффер размером BUF_SIZE!
    char readBuf[BUF_SIZE] = {0};
    int byte = read(_fd, readBuf, BUF_SIZE - 1);
    if (byte < 0) {
        std::string str = std::to_string(_fd) + " read error";
        perror(str.c_str());
        return byte;
    }
    (buf)->resize(byte, ' ');
    std::memcpy((void *) (buf)->c_str(), readBuf, byte);
    return 0;
}

ClientImpl::ClientImpl(int sock, TypeClient typeClient, Buffer *buf) {
    _fd = sock;
    _typeClient = typeClient;
    _buffer = buf;
    _structPollFd.fd = _fd;
    if (typeClient == TypeClient::USER) {
        _structPollFd.events = POLLIN | POLLRDHUP;
    } else if (typeClient == TypeClient::HTTP_SERVER) {
        _structPollFd.events = POLLOUT | POLLIN | POLLRDHUP;
    }
//    _structPollFd.events = POLLOUT | POLLIN;
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

bool ClientImpl::isInClientList() {
    return _isInClientList;
}

void ClientImpl::setInClientList(bool value) {
    _isInClientList = value;
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
