//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"

using namespace ProxyServer;

void ServerImpl::startServer() {
    setPollArr();
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
                    setPollArr();
                    LOG_EVENT("add new client");
                    //TODO client connect: create client + add to _pollSet
                } catch (ConnectException *exception) {
                    std::cerr << exception->what() << std::endl;
                    LOG_ERROR("exception in connect");
                }
            }
        }
    }

}


void ServerImpl::setPollArr() {
    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;

    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        _pollSet[i].fd = (*it)->getFdClient();
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
        if (_pollSet[i].revents & POLLIN) {
            memset(buf, 0, BUF_SIZE);
            int countByteRead = (*it)->readBuf(buf);
            if (countByteRead == 0) {
                isNeedUpdatePollSet = deleteClient(*it, &it);
            } else {
                (*it)->getBuffer()->readRequest(buf);
                if ((*it)->getBuffer()->isReadyConnectHttpServer()) {
                    try {
                        Client *client = _serverSocket->connectToClient
                                ((*it)->getBuffer()->getParseResult().getHostName(), DEFAULT_PORT);
                        client->setBuffer((*it)->getBuffer());
                        client->setPair((*it));
                        (*it)->setPair(client);
                        _clientList.push_back(client);
                        isNeedUpdatePollSet = true;
                    } catch (ConnectException ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
//                        isNeedUpdatePollSet = deleteClient(*it, &it);
                    }
                }
            }
        } else if (_pollSet[i].revents & POLLOUT) {
            if ((*it)->getBuffer()->isReadyToSend()) {
                if (((*it)->getTypeClient() == TypeClient::HTTP_SERVER
                     && (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) ||
                    ((*it)->getTypeClient() == TypeClient::USER
                     && (*it)->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE)) {

                    const char *bufferSend = (*it)->getBuffer()->sendBuf();
                    (*it)->sendBuf(bufferSend);
                    (*it)->getBuffer()->proofSend(bufferSend);
                }
            }
        }

        if ((*it)->getTypeClient() == TypeClient::USER &&
            (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        }
        _pollSet[i].revents = 0;
    }

    if (isNeedUpdatePollSet) {
        setPollArr();
    }
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

bool ServerImpl::deleteClient(Client *client, std::list<Client *>::iterator *iterator) { // TODO norm del!
    if (client->getTypeClient() == TypeClient::USER) {
        LOG_EVENT("user logout");
        if (client->getPair() != NULL) {
            for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
                if ((*it) == client->getPair()) {
                    LOG_EVENT("http server logout with user");
                    _clientList.erase(it);
                    break;
                }
            }
            delete client->getPair();
        }
        (*iterator) = _clientList.erase((*iterator));
        delete client->getBuffer();
        delete client;
        updatePollArr(); // не уверен
        return false;
    } else if (client->getTypeClient() == TypeClient::HTTP_SERVER) {
        LOG_EVENT("http server logout");
        (*iterator) = _clientList.erase((*iterator));
        client->getPair()->setPair(NULL);
        delete client;
        return true;
    }
    return false;
}

void ServerImpl::updatePollArr() {
    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        _pollSet[i].fd = (*it)->getFdClient();
        _pollSet[i].events = POLLIN | POLLOUT;
    }
}
