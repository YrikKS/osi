//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"

using namespace ProxyServer;

void ServerImpl::startServer() {
    while (_isWork) {
        int code = poll(_pollSet, _clientList.size() + 1, TIME_OUT_POLL);

        if (code == -1) {
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            //??
        } else {
            handlingEvent();
            if (poll_set[0].revents & POLLIN) { // poll sock
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
        poll_set[i].fd = (*it)->getFdClient();
        poll_set[i].events = POLLIN;
    }
}

ServerImpl::ServerImpl() {
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
}

void ServerImpl::handlingEvent() {
    int i = 0;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        if (poll_set[i].revents & POLLIN) { // poll sock
            (*it)->readBuf();
        }
    }
}
