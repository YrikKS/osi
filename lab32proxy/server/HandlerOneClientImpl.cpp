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
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
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
                            return 1;
                        }
                    }
                    if ((*it)->getTypeClient() == HTTP_SERVER) {
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                        }
                    } else if ((*it)->getTypeClient() == USER) {
                        if ((*it)->getPair() != NULL) {
                            (*it)->getPair()->setEvents(POLLOUT | POLLIN | POLLRDHUP);
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
                        Client *client = ServerSocketImpl::connectToClient(
                                (*it)->getBuffer()->getParseResult().getHostName(),
                                (*it)->getBuffer()->getParseResult().getPort());
                        client->setBuffer((*it)->getBuffer());
                        client->getBuffer()->setIsServerConnect(true);
                        (*it)->setPair(client);
                        client->addClientToHandlingEvent(*it);
                        client->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                        _clientList.push_front(client);
                    } catch (std::exception &ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
                        deleteClient(&it);
                        pthread_exit(NULL);
                        return 0;
                    }
                }
            }
        }
        if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
            (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK &&
            !(*it)->getBuffer()->isIsAddDataToCash()) {
            deleteClient(&it);
            continue;
//        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
//                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK &&
//                   !(*it)->getBuffer()->isIsDataGetCash()) {
//            deleteClient(&it);
//            continue;
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

                    (*it)->getBuffer()->sendBuf(&buffer);
                    (*it)->sendBuf(&buffer);
                    (*it)->getBuffer()->proofSend(&buffer);

                    if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER && //переключение
                        (*it)->getBuffer()->getStatusHttpServer() == WRITE_RESPONSE_HEADING
                        && !(*it)->getBuffer()->isReadyToSend()) {
                        (*it)->setEvents(POLLIN | POLLRDHUP);
                        std::list<Client *> fromServ = (*it)->getListHandlingEvent();
                        for (auto itList = fromServ.begin();
                             itList != fromServ.end(); itList++) {
                            (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
                        }

                    } else if ((*it)->getTypeClient() == TypeClient::USER && // при отключении сервера
                               (*it)->getBuffer()->getStatusHttpServer() == END_WORK
                               && !(*it)->getBuffer()->isReadyToSend()) {
                        deleteClient(&it);
                        continue;
                    }
                }
            } else {
                (*it)->setEvents(POLLRDHUP);
                continue;
            }
        }

    }
    return 0;
}

void HandlerOneClientImpl::deleteClient(std::list<Client *>::iterator *iterator) {
    LOG_EVENT("logout");
//    std::cout << "dell client " << std::endl;
    if ((**iterator)->getTypeClient() == TypeClient::USER) {
        deleteClientUser(**iterator);
    } else if ((**iterator)->getTypeClient() == TypeClient::HTTP_SERVER) {
        deleteClientServer(**iterator);
    }
    (*iterator) = _clientList.erase((*iterator));
}

void HandlerOneClientImpl::deleteClientUser(Client *client) {
    LOG_EVENT("user logout");
//    std::cout << "user logout" << std::endl;
    if (client->getPair() != NULL) {
        client->getPair()->eraseIt(client);

        if (client->getBuffer() != NULL) {
            if (!client->getBuffer()->isIsAddDataToCash()) {
                for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
                    if (*it == client->getPair())
                        deleteClient(&it);
                }
            }
//            if (!client->getBuffer()->isIsDataGetCash()) {
//                for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
//                    if (*it == client->getPair())
//                        deleteClient(&it);
//                }
//            }
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
//    std::cout << "server logout" << std::endl;
    std::list<Client *> fromServ = client->getListHandlingEvent();
    for (auto itList = fromServ.begin(); itList != fromServ.end(); itList++) {
        (*itList)->setPair(NULL);
        (*itList)->setEvents(POLLOUT | POLLIN | POLLRDHUP);
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
    pthread_mutex_t mutexForCond;
    pthread_cond_t cond;
    if (initializeResources(&mutexForCond, &cond) != SUCCESS) {
        return;
    }
    pthread_mutex_lock(&mutexForCond);
    _client->getBuffer()->getCashElement()->addCondVar(&cond);
    bool run = true;
    while (run) {
        while (_client->getBuffer()->getCashElement()->isIsServerConnected() &&
               !_client->getBuffer()->isReadyToSend()) {
            if (condWait(&mutexForCond, &cond) != SUCCESS) {
                run = false;
                break;
            }
        }
        if (!_client->getBuffer()->getCashElement()->isIsServerConnected()) {
            sendAll();
            break;
        }
        if (_client->getBuffer()->isReadyToSend()) {
            if (sendAll() == FAILURE) {
                break;
            }
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
        return FAILURE;
    }
    return SUCCESS;
}

int HandlerOneClientImpl::sendAll() {
    while (_client->getBuffer()->isReadyToSend()) {
        buffer.clear();
        _client->getBuffer()->sendBuf(&buffer);
        int code = _client->sendBuf(&buffer);
        if (code < 0) {
            return FAILURE;
        }
        _client->getBuffer()->proofSend(&buffer);
    }
}




