//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"
using namespace ProxyServer;

int ClientImpl::getFdClient() {
    return _fd;
}

void ClientImpl::sendBuf(std::shared_ptr<std::string>* buf) {
    write(_fd, (*buf)->c_str(), (*buf)->length()); // Chek!?
//    std::cout << "buf prinf:" << std::endl;
//    buf->printer();
}

void ClientImpl::readBuf(std::shared_ptr<std::string>* buf) { // передать пустой буффер размером BUF_SIZE!
    char readBuf[BUF_SIZE] = {0};
    int byte = read(_fd, readBuf, BUF_SIZE - 1);
//    (*buf)->copy()
        std::cout << "buf read:" << std::endl;
    std::cout.flush();
    (*buf)->clear();
    std::cout << "buf read: clear " << (*buf)->capacity() << " and " << (*buf)->length() << std::endl;
    std::cout.flush();
    std::string str1(readBuf, byte);
    (*buf)->resize(byte);
    std::cout << "buf " << (*buf)->capacity() << " and " << (*buf)->length()
    std::cout.flush();
//    std::cout << "memcopy" << std::endl;
//    std::cout.flush();
    std::memcpy((void *) (*buf)->c_str(), readBuf, byte);
    std::cout << " !!!!!!!!1 buf " << (*buf)->capacity() << " and " << (*buf)->length() << std::endl;
    std::cout.flush();
//    std::cout << "buf = " <<(*buf) << ":end, buf size = " << (*buf)->length() << std::endl;
//    buf->setBinaryString(readBuf, byte);
//    std::cout << "buf read:" << std::endl;
//    buf->printer();
//    std::cout << strlen(buf) << " ws " << byte << std::endl;
//    std::cout.flush();
//    return byte;
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

//Client *ClientImpl::getPair() {
//    return _pair;
//}
//
//void ClientImpl::setPair(Client *pair) {
//    _pair = pair;
//}

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
