//
// Created by kurya on 06.12.2022.
//

#include "NewServerImpl.h"


void ProxyServer::NewServerImpl::startServer() {
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;
    while (true) {
        int code = poll(_pollSet, _clientList.size() + 1, TIME_OUT_POLL);
        saveResultPollSet();
        if (code == -1) {
            LOG_ERROR_WITH_ERRNO("poll error");
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            std::cout << "time out" << std::endl;
        } else {
            handlingEvent();
            if (_pollSet[0].revents & POLLIN) { // poll sock
                _pollSet[0].revents = 0;
                try {
                    _clientList.push_back(_serverSocket->acceptNewClient(_cash));
                    LOG_EVENT("add new client");
                    //TODO client connect: create client + add to _pollSet
                } catch (std::exception *exception) {
                    std::cerr << exception->what() << std::endl;
                    LOG_ERROR("exception in connect");
                }
            }
        }
        setPollSetBeforePoll();
    }
}

void ProxyServer::NewServerImpl::setPollSetBeforePoll() {
    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _pollSet[0].fd = _serverSocket->getFdSocket();
    _pollSet[0].events = POLLIN;

    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement = (*it)->getPollFd();
        _pollSet[i].fd = pollElement.fd;
        _pollSet[i].events = pollElement.events;
        _pollSet[i].revents = pollElement.revents;
    }
}

void ProxyServer::NewServerImpl::saveResultPollSet() {
    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement;
        pollElement.fd = _pollSet[i].fd;
        pollElement.events = _pollSet[i].events;
        pollElement.revents = _pollSet[i].revents;
        (*it)->setPollElement(pollElement);
    }
}

ProxyServer::NewServerImpl::NewServerImpl() {
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
    char data[BUF_SIZE] = {0};
}

ProxyServer::NewServerImpl::~NewServerImpl() {
    _serverSocket->closeSocket();
    delete (ServerSocketImpl *) _serverSocket;
    for (auto &it: _clientList) {
        delete it;
    }
    delete _cash;
}


void ProxyServer::NewServerImpl::handlingEvent() {
    std::cout << "new iteration" << std::endl;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
        std::cout << "start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
        std::string buffer;
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
            (*it)->readBuf(&buffer);
            if (buffer.length() == 0) {
                deleteClient(&it);
                continue;
            } else {
                try {
                    (*it)->getBuffer()->readFromSocket(&buffer);
//                    if ((*it)->getTypeClient() == USER) {
//                        if ((*it)->getBuffer()->getStatusClient() == READ_RESPONSE
//                            && (*it)->getBuffer()->isIsDataGetCash()) {
//                            if ((*it)->getBuffer()->getCashElement()->isCashEnd()) {
//                                (*it)->setEvents(POLLOUT | POLLIN);
//                                continue;
//                            }
//                            else {
//                                findElementWithCurrentCash(*it);
//                                continue;
//                            }
//                        }
//                    }
                    std::cout << "end read from socket" << std::endl;
                    if ((*it)->getTypeClient() == HTTP_SERVER) {
                        std::cout << "try add to list from HTTP_SERVER " << std::endl;
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            if (!(*itList)->isInClientList()) {
                                (*itList)->setInClientList(true);
                                (*itList)->setEvents(POLLOUT | POLLIN);
                                _clientList.push_back(*itList);
                            } else {
//                                (*itList)->setEvents(POLLOUT);
                            }
                        }
                    } else if ((*it)->getTypeClient() == USER) {
                        std::cout << "try add to list from user " << std::endl;
                        if ((*it)->getPair() != NULL) {
                            if (!(*it)->getPair()->isInClientList()) {
                                (*it)->getPair()->setInClientList(true);
                                (*it)->getPair()->setEvents(POLLOUT | POLLIN);
                                _clientList.push_back((*it)->getPair());
                            } else {
//                                (*it)->getPair()->setEvents(POLLOUT);
                            }
                        }
                    }
                } catch (ParseException &ex) {
                    std::cerr << ex.what() << std::endl;
                    LOG_ERROR("send error and disconnect");
                    //TODO disconnect
                }
                if ((*it)->getBuffer()->isReadyConnectHttpServer()) {
                    (*it)->getBuffer()->setReadyConnectHttpServer(false);
                    try {
                        std::cout << "try connect server " << std::endl;
                        Client *client = _serverSocket->connectToClient
                                ((*it)->getBuffer()->getParseResult().getHostName(),
                                 (*it)->getBuffer()->getParseResult().getPort());
                        client->setBuffer((*it)->getBuffer());
                        client->getBuffer()->setIsServerConnect(true);
                        (*it)->setPair(client);
                        client->addClientToHandlingEvent(*it);
                        client->setEvents(POLLOUT | POLLIN);
                        client->setInClientList(true);
                        _clientList.push_back(client);
                        std::cout << "end connect server " << std::endl;
                    } catch (std::exception &ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
                        deleteClient(&it);
                        continue;
                    }
                }
            }
        } else if ((*it)->getPollFd().revents & POLLOUT) {
            (*it)->setReventsZero();
            if ((*it)->getBuffer()->isReadyToSend()) {
                if (((*it)->getTypeClient() == TypeClient::HTTP_SERVER
                     && (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) ||
                    ((*it)->getTypeClient() == TypeClient::USER
                     && (*it)->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE)) {

                    std::cout << "wright to client " << std::endl;
                    (*it)->getBuffer()->sendBuf(&buffer);
                    (*it)->sendBuf(&buffer);
                    (*it)->getBuffer()->proofSend(&buffer);
                    std::cout << "end wright to client " << std::endl;
                    if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                        (*it)->getBuffer()->getStatusHttpServer() == WRITE_RESPONSE_HEADING
                        && !(*it)->getBuffer()->isReadyToSend()) {

                        std::cout << "pereclich " << std::endl;
                        (*it)->setEvents(POLLIN);
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        std::cout << fromServ.size() << std::endl;
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            std::cout << "itearion " << std::endl;
                            if (!(*itList)->isInClientList()) {
                                (*itList)->setInClientList(true);
                                (*itList)->setEvents(POLLOUT | POLLIN);
                                _clientList.push_back(*itList);
                            } else {
                                (*itList)->setEvents(POLLOUT | POLLIN);
                            }
                        }
                        std::cout << "end pereclich " << std::endl;
                    } else if ((*it)->getTypeClient() == TypeClient::USER && // при отключении сервера
                               (*it)->getBuffer()->getStatusClient() == END_WORK
                               && !(*it)->getBuffer()->isReadyToSend()) {
                        deleteClient(&it);
                        continue;
                    } else if ((*it)->getTypeClient() == TypeClient::USER // при отключении сервера
                               && (*it)->getBuffer()->isIsDataGetCash()
                               && (*it)->getBuffer()->getCashElement()->isCashEnd()
                               && !(*it)->getBuffer()->isReadyToSend()) {
                        deleteClient(&it);
                        continue;
                    }
                }
            } else {
                std::cout << "ERASE !! " << std::endl;
                (*it)->setInClientList(false);
                it = _clientList.erase(it);
                continue;
            }
        }
        if ((*it)->getTypeClient() == TypeClient::USER &&
            (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK) {
            deleteClient(&it);
            continue;
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK) {
            deleteClient(&it);
            continue;
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK &&
                   !(*it)->getBuffer()->isReadyToSend()) {
            deleteClient(&it);
            continue;
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK &&
                   (*it)->getBuffer()->isSendEnd()) {
            deleteClient(&it);
            continue;
        }
    }
}

void ProxyServer::NewServerImpl::deleteClient(std::list<ProxyServer::Client *>::iterator *iterator) {
    if ((**iterator)->getTypeClient() == TypeClient::USER) {
        deleteClientUser(**iterator);
    } else if ((**iterator)->getTypeClient() == TypeClient::HTTP_SERVER) {
        deleteClientServer(**iterator);
    }
    (*iterator) = _clientList.erase((*iterator));
    std::cout << "end delete" << std::endl;
}

void ProxyServer::NewServerImpl::deleteClientServer(Client *client) {
    LOG_EVENT("http server logout");
    std::list<Client *> fromServ = client->getListHandlingEvent();
    for (auto itList = fromServ.begin(); itList != fromServ.end(); itList++) {
        (*itList)->setPair(NULL);
        if (!(*itList)->isInClientList()) {
            (*itList)->setInClientList(true);
            _clientList.push_back((*itList));
        }
    }
    if (client->getBuffer() != NULL) {
        if (client->getBuffer()->isIsAddDataToCash()) {
            client->getBuffer()->getCashElement()->setIsServerConnect(false);
        }
        client->getBuffer()->setStatusServer(StatusHttp::END_WORK);
        client->getBuffer()->setIsServerConnect(false);
        if (!client->getBuffer()->isIsClientConnect()) {
            delete client->getBuffer();
            client->setBuffer(NULL);
        }
    }
    delete client;
}

void ProxyServer::NewServerImpl::deleteClientUser(Client *client) {
    LOG_EVENT("user logout");
    if (client->getPair() != NULL) {
        client->getPair()->eraseIt(client);
    }

    if (client->getBuffer() != NULL) {
        if (client->getBuffer()->isIsDataGetCash()) {
            client->getBuffer()->getCashElement()->minusCountUsers();
        }
        client->getBuffer()->setStatusClient(StatusHttp::END_WORK);
        client->getBuffer()->setIsClientConnect(false);
        if (!client->getBuffer()->isIsServerConnect()) {
            delete client->getBuffer();
            client->setBuffer(NULL);
        }
    }
    delete client;
}

void ProxyServer::NewServerImpl::findElementWithCurrentCash(Client *client) {
    for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
        if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER) {
            if ((*it)->getBuffer()->getCashElement() == client->getBuffer()->getCashElement()) {
                (*it)->addClientToHandlingEvent(client);
                break;
            }
        }
    }
}






