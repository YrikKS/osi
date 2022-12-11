//
// Created by kurya on 10.12.2022.
//

#include "HandlerOneClientImpl.h"

using namespace ProxyServer;

void HandlerOneClientImpl::startHandler() {
    while (_clientList.size() > 0) {
        setPollSetBeforePoll();
        int code = poll(_pollSet, _clientList.size(), TIME_OUT_POLL);
        saveResultPollSet();
        if (code == -1) {
            LOG_ERROR_WITH_ERRNO("poll error");
            perror("poll error");
            break;
            //TODO exit
        } else if (code == 0) {
            handlingEvent();
            std::cout << "time out" << std::endl;
        } else {
            handlingEvent();
        }
    }
}


HandlerOneClientImpl::HandlerOneClientImpl(Client *client) {
    _clientList.push_front(client);
}

HandlerOneClientImpl::~HandlerOneClientImpl() {
    for (auto &it: _clientList) {
        delete it;
    }
}

void HandlerOneClientImpl::setPollSetBeforePoll() {
    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, 2 * sizeof(struct pollfd));

    int i = 0;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement = (*it)->getPollFd();
        _pollSet[i].fd = pollElement.fd;
        _pollSet[i].events = pollElement.events;
        _pollSet[i].revents = pollElement.revents;
    }
}


void HandlerOneClientImpl::handlingEvent() {
    for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
        if ((*it)->getPollFd().revents & POLLRDHUP) {
            deleteClient(&it);
            continue;
        }
        std::cout << "start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
        std::string buffer;
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
            int code = (*it)->readBuf(&buffer);
            if(code < 0) {
                continue;
            }
            if (buffer.length() == 0) {
                deleteClient(&it);
                continue;
            } else {
                try {
                    (*it)->getBuffer()->readFromSocket(&buffer);
                    if ((*it)->getTypeClient() == USER) {
                        if ((*it)->getBuffer()->getStatusClient() == READ_RESPONSE
                            && (*it)->getBuffer()->isIsDataGetCash()) {
                            if ((*it)->getBuffer()->getCashElement()->isCashEnd()) {
                                (*it)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                                continue;
                            } else {
                                (*it)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
//                                findElementWithCurrentCash(*it);
                                (*it)->setInClientList(false);
                                it = _clientList.erase(it);
                                continue;
                            }
                        }
                    }
                    std::cout << "end read from socket" << std::endl;
                    if ((*it)->getTypeClient() == HTTP_SERVER) {
                        std::cout << "try add to list from HTTP_SERVER " << std::endl;
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            if (!(*itList)->isInClientList()) {
                                (*itList)->setInClientList(true);
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                                _clientList.push_back(*itList);
                            } else {
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                            }
                        }
                    } else if ((*it)->getTypeClient() == USER) {
                        std::cout << "try add to list from user " << std::endl;
                        if ((*it)->getPair() != NULL) {
                            if (!(*it)->getPair()->isInClientList()) {
                                (*it)->getPair()->setInClientList(true);
                                (*it)->getPair()->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                                _clientList.push_back((*it)->getPair());
                            } else {
                                (*it)->getPair()->setEvents(POLLOUT | POLLIN | POLLRDHUP);
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
                        Client* client = ServerSocketImpl().connectToClient(
                                (*it)->getBuffer()->getParseResult().getHostName(),
                                (*it)->getBuffer()->getParseResult().getPort());
//                        Client *client = ServerSocket->->connectToClient
//                                ((*it)->getBuffer()->getParseResult().getHostName(),
//                                 (*it)->getBuffer()->getParseResult().getPort());
                        client->setBuffer((*it)->getBuffer());
                        client->getBuffer()->setIsServerConnect(true);
                        (*it)->setPair(client);
                        client->addClientToHandlingEvent(*it);
                        client->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                        client->setInClientList(true);
                        _clientList.push_front(client);
                        std::cout << "end connect server " << std::endl;
                    } catch (std::exception &ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
                        deleteClient(&it);
                        continue;
                    }
                }
            }
        }
        if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
            (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK &&
            !(*it)->getBuffer()->isIsDataGetCash()) {
            deleteClient(&it);
            continue;
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
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
        if ((*it)->getPollFd().revents & POLLOUT) {
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
//                    std::cout << "end wright to client " << std::endl;
                    if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                        (*it)->getBuffer()->getStatusHttpServer() == WRITE_RESPONSE_HEADING
                        && !(*it)->getBuffer()->isReadyToSend()) {

                        std::cout << "pereclich " << std::endl;
                        (*it)->setEvents(POLLIN | POLLRDHUP);
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        std::cout << fromServ.size() << std::endl;
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            std::cout << "itearion " << std::endl;
                            if (!(*itList)->isInClientList()) {
                                (*itList)->setInClientList(true);
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                                _clientList.push_back(*itList);
                            } else {
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
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
                (*it)->getPair()->setInClientList(true);
                (*it)->setEvents(POLLRDHUP);
                continue;
            }
        }

    }
}

void HandlerOneClientImpl::deleteClient(std::list<Client *>::iterator *iterator) {
    std::cout << "dell client " << std::endl;
    if ((**iterator)->getTypeClient() == TypeClient::USER) {
        deleteClientUser(**iterator);
    } else if ((**iterator)->getTypeClient() == TypeClient::HTTP_SERVER) {
        deleteClientServer(**iterator);
    }
    (*iterator) = _clientList.erase((*iterator));
}

void HandlerOneClientImpl::deleteClientUser(Client *client) {
    LOG_EVENT("user logout");
    if (client->getPair() != NULL) {
        client->getPair()->eraseIt(client);

        if (client->getBuffer() != NULL) {
            if (!client->getBuffer()->isIsDataGetCash()) {
                for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
                    if (*it == client->getPair())
                        deleteClient(&it);
                }
            }
        }
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

void HandlerOneClientImpl::deleteClientServer(Client *client) {
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

void HandlerOneClientImpl::saveResultPollSet() {
    int i = 0;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement;
        pollElement.fd = _pollSet[i].fd;
        pollElement.events = _pollSet[i].events;
        pollElement.revents = _pollSet[i].revents;
        (*it)->setPollElement(pollElement);
    }
}



