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
            LOG_ERROR("poll error");
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            //??
        } else {
            handlingEvent();
            if (_pollSet[0].revents & POLLIN) { // poll sock
                _pollSet[0].revents = 0;
                LOG_EVENT("add new client");
                _clientList.push_back(_serverSocket->acceptNewClient());
                updatePollFd();
                //TODO client connect: create client + add to _pollSet
            }
        }
    }

}


void ServerImpl::updatePollFd() {
//    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;

    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
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
    char buf[1024] = {0};
    bool isNeedUpdatePollSet = false;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        if (_pollSet[i].revents & POLLIN) { // poll sock
            _pollSet[i].revents = 0;
            int countByteRead = (*it)->readBuf(buf);
            if(countByteRead == 0) {
                LOG_EVENT("user logout");
                _clientList.erase(it);
                delete (*it);
//                isNeedUpdatePollSet = true;
                updatePollFd();
            } else {

            }
        }
    }
    LOG_EVENT("end handlingEvent");
}

void ServerImpl::handlingReadBuf(char* buf) {
    std::cout << "handlingEvent" << std::endl;
}
