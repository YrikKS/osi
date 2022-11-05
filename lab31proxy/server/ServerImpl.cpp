//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"

using namespace ProxyServer;

void ServerImpl::startServer() {
    updatePollFd();
    std::cout << "start server" << std::endl;
    while (_isWork) {
        int code = poll(_pollSet, _clientList.size() + 1, TIME_OUT_POLL);

        if (code == -1) {
            LOG_ERROR_WITH_ERRNO("poll error");
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            //??
        } else {
            handlingEvent();
            if (_pollSet[0].revents & POLLIN) { // poll sock
                _pollSet[0].revents = 0;
                try {
                    _clientList.push_back(_serverSocket->acceptNewClient());
                    updatePollFd();
                    //TODO client connect: create client + add to _pollSet
                } catch (ConnectException *exception) {
                    std::cerr << exception->what() << std::endl;
                    LOG_ERROR("exception is close");
                }

                LOG_EVENT("add new client");
            }
        }
    }

}


void ServerImpl::updatePollFd() {
    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;

    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        _pollSet[i].fd = (*it)->getFdClient();
        _pollSet[i].events = POLLIN;
        _pollSet[i].events = POLLIN | POLLOUT;
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
            memset(buf, 0, BUF_SIZE);
            int countByteRead = (*it)->readBuf(buf);
            if (countByteRead == 0) {
                LOG_EVENT("user logout");
                delete (*it);
                it = _clientList.erase(it);
                isNeedUpdatePollSet = true;
            } else {
                handlingReadBuf(buf, *it);
            }
        }
        _pollSet[i].revents = 0;
    }
    if (isNeedUpdatePollSet) {
        updatePollFd();
    }
}

void ServerImpl::handlingReadBuf(char *buf, Client *client) {
    LOG_EVENT("Handling event read");
    std::cout << "handlingEvent" << std::endl;
    std::cout << buf << std::endl;
    if (client->getClientData()->getStatusRequest() == STATUS_REQUEST::READ_REQUEST_HEADING) {
        int posEndHeading = 0;
        if (ParserImpl::findEndHeading(buf, &posEndHeading) == ResultPars::END_REQUEST_HEADING) {
            client->getClientData()->setStatusRequest(STATUS_REQUEST::READ_REQUEST_BODY);
            client->getClientData()->setRequestHeading(
                    client->getClientData()->getRequestHeading() + std::string(buf).substr(0, posEndHeading));
            client->getClientData()->setResultParseHeading(
                    ParserImpl::parsingHeading(client->getClientData()->getRequestHeading()));

            client->getClientData()->setRequestBody(std::string(buf).substr(posEndHeading));
            //TODO может быть подругому ???

            Client *httpServer = _serverSocket->connectToClient(
                    client->getClientData()->getResultParseHeading()->getHostName(),
                    DEFAULT_PORT);

            _clientList.push_back(httpServer);
            updatePollFd();

//            std::cout << "try send ::: " << client->getClientData()->getRequestHeading() << std::endl;
            char newBuf[1024] = {0};
            httpServer->sendBuf((client->getClientData()->getRequestHeading()).data());
            std::cout << httpServer->readBuf(newBuf) << std::endl;

        } else {
            client->getClientData()->setRequestHeading(client->getClientData()->getRequestHeading() + std::string(buf));
        }

    }
//    if (client->getStatusRequest() == STATUS_REQUEST::READ_REQUEST_BODY) {
//
//    }
}

ServerImpl::~ServerImpl() {
    std::cout << "clea" << std::endl;
    std::cout.flush();
    _serverSocket->closeSocket();
    std::cout << "clea1" << std::endl;
    std::cout.flush();
    delete (ServerSocketImpl *) _serverSocket;
    std::cout << "clea2" << std::endl;
    std::cout.flush();
    for (auto &it: _clientList) {
        delete it;
    }
    std::cout << "clea3" << std::endl;
    std::cout.flush();
}