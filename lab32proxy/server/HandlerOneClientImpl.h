//
// Created by kurya on 10.12.2022.
//

#ifndef LAB32PROXY_HANDLERONECLIENTIMPL_H
#define LAB32PROXY_HANDLERONECLIENTIMPL_H

#include "HandlerOneClient.h"
#include "../logger/Logger.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <list>
#include <memory>
#include "../Constants.h"
#include "../cash/CashImpl.h"
#include "../client/Client.h"
#include "../socket/ServerSocket.h"
#include "../socket/ServerSocketImpl.h"

#define SUCCESS 0
#define FAILURE 1

namespace ProxyServer {
    class HandlerOneClientImpl : public HandlerOneClient {
    public:
        HandlerOneClientImpl(Client* client);

        void startHandler() override;

        ~HandlerOneClientImpl() override;

    private:
        void saveResultPollSet();
        void setPollSetBeforePoll();
        bool handlingEvent();
        void deleteClient(std::list<Client*>::iterator* iterator);
        void deleteClientUser(Client* client);
        void deleteClientServer(Client* client);

        void getFromCash();
        bool initializeResources(pthread_mutex_t* mutex, pthread_cond_t* cond);
        bool deleteResources(pthread_mutex_t* mutex, pthread_cond_t* cond);
        bool condWait(pthread_mutex_t* mutex, pthread_cond_t* cond);
        int sendAll();

        std::string buffer;
        Client* _client;
        struct pollfd _pollSet[2];
        std::list<Client*> _clientList;

    };
}


#endif //LAB32PROXY_HANDLERONECLIENTIMPL_H
