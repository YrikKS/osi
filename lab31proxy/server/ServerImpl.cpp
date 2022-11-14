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
            //??
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
//        (*it)->setPollElement(&(_pollSet[i]));
    }
}


ServerImpl::ServerImpl() {
    memset(_pollSet, 0, MAX_COUNT_CONNECTIONS * sizeof(struct pollfd));
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
    _binaryString = new BinaryString;
}

void ServerImpl::handlingEvent() {
    int i = 1;
    bool isNeedUpdatePollSet = false;
    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
        _binaryString->deleteData();
        if ((*it)->getPollFd().revents & POLLIN) {
            (*it)->setReventsZero();
            (*it)->readBuf(_binaryString);
            if (_binaryString->getLength() == 0) {
                isNeedUpdatePollSet = deleteClient(*it, &it);
            } else {
                try {
                    (*it)->getBuffer()->readRequest(_binaryString);
                } catch (ParseException ex) {
                    LOG_ERROR("send error and disconnect");

                }
                if ((*it)->getBuffer()->isReadyConnectHttpServer()) {
                    try {
                        Client *client = _serverSocket->connectToClient
                                ((*it)->getBuffer()->getParseResult().getHostName(),
                                 (*it)->getBuffer()->getParseResult().getPort());
                        client->setBuffer((*it)->getBuffer());
                        client->setPair((*it));
                        (*it)->setPair(client);
                        _clientList.push_back(client);
                        isNeedUpdatePollSet = true;
                    } catch (std::exception ex) {
                        std::cerr << ex.what() << std::endl;
                        LOG_ERROR("can't connect to http server");
                        isNeedUpdatePollSet = deleteClient(*it, &it);
                    }
                }
            }
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
                   (*it)->getBuffer()->getStatusClient() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
//            break;
        } else if ((*it)->getTypeClient() == TypeClient::HTTP_SERVER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK) {
            isNeedUpdatePollSet = deleteClient(*it, &it);
//            break;
        } else if ((*it)->getTypeClient() == TypeClient::USER &&
                   (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::END_WORK &&
                   !(*it)->getBuffer()->isReadyToSend()) {
            isNeedUpdatePollSet = deleteClient(*it, &it);

        } else if ((*it)->getPollFd().revents & POLLOUT) {
            (*it)->setReventsZero();
//            std::cout << (*it)->getTypeClient() << std::endl;
            if ((*it)->getBuffer()->isReadyToSend()) {
                if (((*it)->getTypeClient() == TypeClient::HTTP_SERVER
                     && (*it)->getBuffer()->getStatusHttpServer() == StatusHttp::READ_REQUEST) ||
                    ((*it)->getTypeClient() == TypeClient::USER
                     && (*it)->getBuffer()->getStatusClient() == StatusHttp::READ_RESPONSE)) {

                    (*it)->getBuffer()->sendBuf(_binaryString);
                    (*it)->sendBuf(_binaryString);
                    (*it)->getBuffer()->proofSend(_binaryString);
                }
            } else {
//                (*it)->setReventsZero();
            }
        }
    }

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
    delete _binaryString;
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
//        updatePollArr(); // не уверен
        return true;
    } else if (client->getTypeClient() == TypeClient::HTTP_SERVER) {
        LOG_EVENT("http server logout");
        (*iterator) = _clientList.erase((*iterator));
        if (client->getPair() != NULL) {
            client->getPair()->setPair(NULL);
        }
        client->getBuffer()->setStatusBuf(StatusHttp::END_WORK);
        delete client;
        return true;
    }
    return false;
}

//void ServerImpl::updatePollArr() {
//    int i = 1;
//    for (auto it = _clientList.begin(); it != _clientList.end(); it++, i++) {
//        _pollSet[i].fd = (*it)->getFdClient();
//        _pollSet[i].events = POLLIN | POLLOUT;
//        (*it)->setPollElement(&(_pollSet[i]));
//    }
//}
