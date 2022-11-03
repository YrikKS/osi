//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"

using namespace ProxyServer;

void ServerImpl::startServer() {
    updatePollFd();
    while (_isWork) {
        int code = poll(_pollSet, _clientList.size() + 1, TIME_OUT_POLL);

        if (code == -1) {
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            //??
        } else {
            handlingEvent();
            if (_pollSet[0].revents & POLLIN) { // poll sock
                _pollSet[0].revents = 0;
                std::cout << "add new client" << std::endl;
                _clientList.push_back(_serverSocket->acceptNewClient());
                updatePollFd();
                //TODO client connect: create Client + add to _pollSet
            }
        }
    }

}


void ServerImpl::updatePollFd() {
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;

    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        std::cout << "updatePollFd" << std::endl;
        std::cout << "client fd = " << (*it)->getFdClient() << std::endl;
        _pollSet[i].fd = (*it)->getFdClient();
        _pollSet[i].events = POLLIN;
    }
}

ServerImpl::ServerImpl() {
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
}

void ServerImpl::handlingEvent() {
    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        if (_pollSet[i].revents & POLLIN) { // poll sock
            _pollSet[i].revents = 0;
            std::cout << "read" << std::endl;
            (*it)->readBuf();
        }
    }
}
