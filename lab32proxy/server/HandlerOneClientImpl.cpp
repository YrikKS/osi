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
            int status = handlingEvent();
            if (status) {
                getFromCash();
                deleteClientUser(_client);
                pthread_exit(NULL);
            }
            std::cout << "time out" << std::endl;
        } else {
            int status = handlingEvent();
            if (status) {
                getFromCash();
                deleteClientUser(_client);
                pthread_exit(NULL);
            }
        }
    }
}


HandlerOneClientImpl::HandlerOneClientImpl(Client *client) {
    _clientList.push_front(client);
    _client = client;
}

HandlerOneClientImpl::~HandlerOneClientImpl() {
//    for (auto &it: _clientList) {
//        delete it;
//    }
}

void HandlerOneClientImpl::setPollSetBeforePoll() {
//    LOG_EVENT("update pollSet");
    memset(_pollSet, 0, 2 * sizeof(struct pollfd));

    int i = 0;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement = (*it)->getPollFd();
        _pollSet[i].fd = pollElement.fd;
        _pollSet[i].events = pollElement.events;
        _pollSet[i].revents = pollElement.revents;
    }
}

bool HandlerOneClientImpl::handlingEvent() {
    for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
        buffer.clear();
        if ((*it)->getPollFd().revents & POLLRDHUP) {
            deleteClient(&it);
            continue;
        }
        if ((*it)->getTypeClient() == USER) {
            std::cout << "user start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
        } else {
            std::cout << "server start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
        }
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
//            std::cout << "read from sock == " << (*it)->getFdClient() << std::endl;
            int code = (*it)->readBuf(&buffer);
            if (code < 0) {
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
//                            if ((*it)->getBuffer()->getCashElement()->isCashEnd()) {
//                                return 1;
//                                (*it)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
//                                continue;
//                            }
                            return 1;
                        }
                    }
                    std::cout << "end read from socket" << std::endl;
                    if ((*it)->getTypeClient() == HTTP_SERVER) {
//                        std::cout << "try add to list from HTTP_SERVER " << std::endl;
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
//                        for (auto itList = fromServ.begin();
//                             itList != fromServ.end(); itList++) {
                        _client->setEvents(POLLOUT | POLLIN | POLLRDHUP);
//                        }
                    } else if ((*it)->getTypeClient() == USER) {
//                        std::cout << "try add to list from user " << std::endl;
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
//                        std::cout << "try connect server " << std::endl;
                        Client *client = ServerSocketImpl::connectToClient(
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
//                        std::cout << "end connect server " << std::endl;
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
            if ((*it)->getTypeClient() == USER) {
                std::cout << "2user start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
            }
            (*it)->setReventsZero();
            if ((*it)->getTypeClient() == USER) {
                std::cout << "3user start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
            }
            if ((*it)->getBuffer()->isReadyToSend()) {
                if (((*it)->getTypeClient() == TypeClient::HTTP_SERVER
                     && (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) ||
                    ((*it)->getTypeClient() == TypeClient::USER
                     && (*it)->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE)) {

                    if ((*it)->getTypeClient() == USER) {
                        std::cout << "4user start for list size : " << _clientList.size() << " silka: " << *it << std::endl;
                    }
//                    if ((*it)->getBuffer()->getCashElement() != NULL)
//                        pthread_mutex_lock((*it)->getBuffer()->getCashElement()->getMutex());
                    std::cout << "wright to client " << std::endl;
                    (*it)->getBuffer()->sendBuf(&buffer);

                    std::cout << "main send buff end to client " << std::endl;
                    (*it)->sendBuf(&buffer);
                    std::cout << "wright to socket to client " << std::endl;
                    (*it)->getBuffer()->proofSend(&buffer);
//                    if ((*it)->getBuffer()->getCashElement() != NULL)
//                        pthread_mutex_unlock((*it)->getBuffer()->getCashElement()->getMutex());

                    std::cout << "proof send end wright to client " << std::endl;
                    if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                        (*it)->getBuffer()->getStatusHttpServer() == WRITE_RESPONSE_HEADING
                        && !(*it)->getBuffer()->isReadyToSend()) {

//                        std::cout << "pereclich " << std::endl;
                        (*it)->setEvents(POLLIN | POLLRDHUP);
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
//                        std::cout << fromServ.size() << std::endl;
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
//                            std::cout << "itearion " << std::endl;
                            if (!(*itList)->isInClientList()) {
                                (*itList)->setInClientList(true);
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                                _clientList.push_back(*itList);
                            } else {
                                (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                            }
                        }
//                        std::cout << "end pereclich " << std::endl;
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
                    std::cout << "end iteration :" << (*it) << std::endl;
                }
            } else {
                (*it)->getPair()->setInClientList(true);
                (*it)->setEvents(POLLRDHUP);
                continue;
            }
        }

    }
    return 0;
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

void HandlerOneClientImpl::getFromCash() {
    std::cout << "start getting" << std::endl;
    pthread_mutex_t mutexForCond;
    pthread_cond_t cond;
    if (initializeResources(&mutexForCond, &cond) != SUCCESS) {
        return;
    }
    pthread_mutex_lock(&mutexForCond);
    _client->getBuffer()->getCashElement()->addCondVar(&cond);
    bool run = true;
    std::cout << &mutexForCond << std::endl;
    std::cout << &cond << std::endl;
    while (run) {
        std::cout << "start getting in while" << std::endl;
        while (_client->getBuffer()->getCashElement()->isIsServerConnected() &&
               !_client->getBuffer()->isReadyToSend()) {
            std::cout << "cash sleep" << std::endl;
            if (condWait(&mutexForCond, &cond) != SUCCESS) {
                run = false;
                break;
            }
        }

        std::cout << "cash not sleep" << std::endl;
        if (!_client->getBuffer()->getCashElement()->isIsServerConnected()) {
            sendAll();
            std::cout << "one send all" << std::endl;
            break;
        }
        if (_client->getBuffer()->isReadyToSend()) {
            std::cout << "start send all" << std::endl;
            sendAll();
        }
    }
    pthread_mutex_unlock(&mutexForCond);
    _client->getBuffer()->getCashElement()->dellCondVar(&cond);
    deleteResources(&mutexForCond, &cond);
}

bool HandlerOneClientImpl::initializeResources(pthread_mutex_t *mutex, pthread_cond_t *cond) {
    errno = pthread_mutex_init(mutex, NULL);
    if (errno != SUCCESS) {
        perror("mutex init");
        return FAILURE;
    }

    errno = pthread_cond_init(cond, NULL);
    if (errno != SUCCESS) {
        pthread_mutex_destroy(mutex);
        perror("cond init");
        return FAILURE;
    }
    return SUCCESS;
}

bool HandlerOneClientImpl::deleteResources(pthread_mutex_t *mutex, pthread_cond_t *cond) {
    errno = pthread_mutex_destroy(mutex);
    if (errno != SUCCESS) {
        perror("mutex destroy");
        return FAILURE;
    }

    errno = pthread_cond_destroy(cond);
    if (errno != SUCCESS) {
        pthread_mutex_destroy(mutex);
        perror("cond destroy");
        return FAILURE;
    }
    return SUCCESS;
}

bool HandlerOneClientImpl::condWait(pthread_mutex_t *mutex, pthread_cond_t *cond) {
    errno = pthread_cond_wait(cond, mutex);
    if (errno != SUCCESS) {
        perror("wait error");
//        pthread_mutex_unlock(mutex);
//        deleteResources(mutex, cond);
        return FAILURE;
    }
    return SUCCESS;
}

void HandlerOneClientImpl::sendAll() {
    while (_client->getBuffer()->isReadyToSend()) {
//        if (_client->getBuffer()->getCashElement() != NULL)
//            pthread_mutex_lock(_client->getBuffer()->getCashElement()->getMutex());
//        std::string buf;
//        buf.resize(BUF_SIZE);
buffer.clear();
        std::cout << "child start send" << std::endl;
        _client->getBuffer()->sendBuf(&buffer);
        std::cout << "child send in socket" << std::endl;
        _client->sendBuf(&buffer);
        std::cout << "child end send in socket" << std::endl;
        _client->getBuffer()->proofSend(&buffer);
        std::cout << "child end proof" << std::endl;
//        if (_client->getBuffer()->getCashElement() != NULL)
//            pthread_mutex_unlock(_client->getBuffer()->getCashElement()->getMutex());

    }
    std::cout << "end send cash in iteration" << std::endl;
}




