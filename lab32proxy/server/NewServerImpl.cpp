//
// Created by kurya on 06.12.2022.
//

#include "NewServerImpl.h"
#include <pthread.h>

using namespace ProxyServer;

void NewServerImpl::startServer() {
    std::list<ArgsForThread> listFd;
//    std::string a;
//    std::cin >> a;
//    std::cout << a << std::endl;
//    close(0);
    while (true) {
        try {
            int sock = _serverSocket->acceptNewClientSock();
//            if(sock == 0) {
//                continue;
//            }
            listFd.push_front(ArgsForThread(sock, _cash));
//            std::cout << "read from == " << listFd.begin()->getSock() << std::endl;
            pthread_t pthread;
            errno = pthread_create(&pthread, NULL, &NewServerImpl::startingMethodForThread,
                                   (void *) &(*listFd.begin()));
            if (errno != SUCCESS) {
                perror("pthread_create error");
                continue;
                errno = SUCCESS;
            }
            pthread_join(pthread, NULL);
//            errno = pthread_detach(pthread);
//            if (errno != SUCCESS) {
//                perror("pthread_create error");
//                errno = SUCCESS;
//                continue;
//            }
        } catch (std::exception *exception) {
            std::cerr << exception->what() << std::endl;
            LOG_ERROR("exception in connect");
        }
    }
}

void *NewServerImpl::startingMethodForThread(void *args) {
    ArgsForThread *argsForThread = (ArgsForThread *) args;
//    std::cout << "socke == " << argsForThread->getSock() << std::endl;
    Client *client = new ClientImpl(argsForThread->getSock(), TypeClient::USER,
                                    new BufferImpl(argsForThread->getCash()));
    client->getBuffer()->setIsClientConnect(true);
    std::cout << "client connect!!!!!!!!!!!!!!!!!!!!! " << client->getFdClient() << std::endl;
    HandlerOneClientImpl handlerOneClient = HandlerOneClientImpl(client);
    handlerOneClient.startHandler();
    std::cout << "thread end work" << std::endl;
//    sleep(1000);
}

NewServerImpl::NewServerImpl() {
    _serverSocket = new ServerSocketImpl();
    _serverSocket->connectSocket();
    _cash = new CashImpl();
    pthread_mutex_init(&mutexForServer, NULL);
//    char data[BUF_SIZE] = {0};
}

NewServerImpl::~NewServerImpl() {
    _serverSocket->closeSocket();
    delete (ServerSocketImpl *) _serverSocket;
    delete _cash;
}


//void NewServerImpl::handlingEvent() {
//}


