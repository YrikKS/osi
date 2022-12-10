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
            std::cout << "all okey 1" << std::endl;
            errno = pthread_create(&pthread, NULL, &NewServerImpl::startingMethodForThread, &client);
            if (errno != SUCCESS) {
                perror("pthread_create error");
//            exit(errno);
            }
            std::cout << "all okey 2" << std::endl;
        } catch (std::exception *exception) {
            std::cerr << exception->what() << std::endl;
            LOG_ERROR("exception in connect");
        }
    }
}

void *NewServerImpl::startingMethodForThread(void *args) {
    std::cout << "all okey 03" << std::endl;
    Client* client = (Client *) args;
    std::cout << "all okey 4" << std::endl;
    std::cout << "client connect " << client->getTypeClient() << std::endl;
    std::cout << "all okey 5" << std::endl;
//    return nullptr;
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


void NewServerImpl::handlingEvent() {

}


