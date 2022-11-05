//
// Created by kurya on 03.11.2022.
//

#include "ClientImpl.h"

int ProxyServer::ClientImpl::getFdClient() {
    return _fd;
}

void ProxyServer::ClientImpl::sendBuf(const char *buf) {
    std::cout << "wright : " << buf << std::endl;
    write(_fd, buf, BUF_SIZE);
}

int ProxyServer::ClientImpl::readBuf(char *buf) {
    int byte = read(_fd, buf, BUF_SIZE);
//    std::cout << buf << std::endl;
    return byte;
}

ProxyServer::ClientImpl::ClientImpl(int sock, StatusHttp statusRequest, TypeClient typeClient) {
    _fd = sock;
    _typeClient = typeClient;
    _clientData = new ClientDataImpl(statusRequest);
}

ProxyServer::ClientImpl::~ClientImpl() {
    LOG_EVENT("destructor client");
    close(_fd);
    delete _clientData;
}

ProxyServer::ClientData* ProxyServer::ClientImpl::getClientData() {
    return _clientData;
}

ProxyServer::Client *ProxyServer::ClientImpl::getPair() {
    return _pair;
}

void ProxyServer::ClientImpl::setPair(ProxyServer::Client *pair) {
    ClientImpl::_pair = pair;
}

ProxyServer::TypeClient ProxyServer::ClientImpl::getTypeClient() {
    return _typeClient;
}
