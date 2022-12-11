//
// Created by kurya on 06.12.2022.
//

#include "NewServerImpl.h"
#include <pthread.h>

using namespace ProxyServer;

void NewServerImpl::startServer() {
    while (true) {
        try {
            Client *client = _serverSocket->acceptNewClient(_cash);
            pthread_t pthread;
            errno = pthread_create(&pthread, NULL, &NewServerImpl::startingMethodForThread, (void *) client);
            if (errno != SUCCESS) {
                perror("pthread_create error");
                continue;
                errno = SUCCESS;
            }

            errno = pthread_detach(pthread);
            if (errno != SUCCESS) {
                perror("pthread_create error");
                errno = SUCCESS;
                continue;
            }
        } catch (std::exception *exception) {
            std::cerr << exception->what() << std::endl;
            LOG_ERROR("exception in connect");
        }
    }
}

void *NewServerImpl::startingMethodForThread(void *args) {
    Client *client = (Client *) args;
    std::cout << "client connect " << client->getTypeClient() << std::endl;
    HandlerOneClientImpl handlerOneClient = HandlerOneClientImpl(client);
    handlerOneClient.startHandler();
}

NewServerImpl::NewServerImpl() {
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
//    char data[BUF_SIZE] = {0};
}

NewServerImpl::~NewServerImpl() {
    _serverSocket->closeSocket();
    delete (ServerSocketImpl *) _serverSocket;
    delete _cash;
}


//void NewServerImpl::handlingEvent() {
//}


