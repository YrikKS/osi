//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"
#include "../cash/CashImpl.h"

using namespace ProxyServer;

void ServerImpl::startServer() {
    configuratePollArr();
    std::cout << "start server" << std::endl;
    while (_isWork) {
        int code = poll(_pollSet, _clientList.size() + 1, TIME_OUT_POLL);
        setPollElements();
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
                    configuratePollArr();
                    LOG_EVENT("add new client");
                    //TODO client connect: create client + add to _pollSet
                } catch (std::exception *exception) {
                    std::cerr << exception->what() << std::endl;
                    LOG_ERROR("exception in connect");
                }
            }
        }
    }

}

void ServerImpl::setPollElements() {
    int i = 1;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        struct pollfd pollElement;
        pollElement.fd = _pollSet[i].fd;
        pollElement.events = _pollSet[i].events;
        pollElement.revents = _pollSet[i].revents;
        (*it)->setPollElement(pollElement);
    }
}


void ServerImpl::configuratePollArr() {
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


ServerImpl::ServerImpl() {
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
    char data[BUF_SIZE] = {0};
}

void ServerImpl::handlingEvent() {
    bool isNeedUpdatePollSet = false;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++) {
        std::string buffer;
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
            (*it)->readBuf(&buffer);
            if (buffer.length() == 0) {
                isNeedUpdatePollSet = deleteClient(*it, &it);
            } else {
                try {
                    (*it)->getBuffer()->readFromSocket(&buffer);
                } catch (ParseException &ex) {
                    std::cerr << ex.what() << std::endl;
                    LOG_ERROR("send error and disconnect");
                    //TODO disconnect
                }
                if ((*it)->getBuffer()->isReadyConnectHttpServer()) {
                    (*it)->getBuffer()->setReadyConnectHttpServer(false);
                    try {
                        Client *client = _serverSocket->connectToClient
                                ((*it)->getBuffer()->getParseResult().getHostName(),
                                 (*it)->getBuffer()->getParseResult().getPort());
                        client->setBuffer((*it)->getBuffer());
                        client->getBuffer()->setIsServerConnect(true);
                        (*it)->setPair(client);
                        client->setPair(*it);
                        client->setEvents(POLLOUT);
                        _clientList.push_back(client);
                        isNeedUpdatePollSet = true;
                    } catch (std::exception &ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
                        isNeedUpdatePollSet = deleteClient(*it, &it);
                    }
                }
            }
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK &&
                   !(*it)->getBuffer()->isReadyToSend()) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK &&
                   (*it)->getBuffer()->isSendEnd()) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
        } else if ((*it)->getPollFd().revents & POLLOUT) {
            (*it)->setReventsZero();
            if ((*it)->getBuffer()->isReadyToSend()) {
                if (((*it)->getTypeClient() == TypeClient::HTTP_SERVER
                     && (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) ||
                    ((*it)->getTypeClient() == TypeClient::USER
                     && (*it)->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE)) {

                    (*it)->getBuffer()->sendBuf(&buffer);
                    (*it)->sendBuf(&buffer);
                    (*it)->getBuffer()->proofSend(&buffer);

                }
            } else {
//                (*it)->setReventsZero();
            }
        }
    }
//    for (auto & it : _clientList) {
//        changePollEventForClient(it);
//    }
    if (isNeedUpdatePollSet) {
        configuratePollArr();
    }
}

ServerImpl::~ServerImpl() {
    _serverSocket->closeSocket();
    delete (ServerSocketImpl *) _serverSocket;
    for (auto &it: _clientList) {
        delete it;
    }
    delete _cash;
}


bool ServerImpl::deleteClient(Client *client, std::list<Client *>::iterator *iterator) { // TODO norm del!
    if (client->getTypeClient() == TypeClient::USER) {
        LOG_EVENT("user logout");

        (*iterator) = _clientList.erase((*iterator));

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
        return true;
    } else if (client->getTypeClient() == TypeClient::HTTP_SERVER) {
        LOG_EVENT("http server logout");
        (*iterator) = _clientList.erase((*iterator));

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
        return true;
    }
    return false;
}

void ServerImpl::changePollEventForClient(Client *client) {
    if (client->getTypeClient() == TypeClient::USER) {
        if (client->getBuffer()->getStatusClient() == StatusHttp::WRITE_REQUEST_BODY ||
            client->getBuffer()->getStatusClient() == StatusHttp::WRITE_REQUEST_HEADING) {
            client->setEvents(POLLIN);
        } else if (client->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE) {
            client->setEvents(POLLOUT | POLLIN);
        }
    } else if (client->getTypeClient() == TypeClient::HTTP_SERVER) {
        if (client->getBuffer()->getStatusHttpServer() == StatusHttp::WRITE_RESPONSE_BODY ||
            client->getBuffer()->getStatusHttpServer() == StatusHttp::WRITE_RESPONSE_HEADING) {
            client->setEvents(POLLIN);
        } else if (client->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) {
            client->setEvents(POLLOUT | POLLIN);
        }
    }
}
