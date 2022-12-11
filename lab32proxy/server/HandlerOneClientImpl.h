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

namespace ProxyServer {
    class HandlerOneClientImpl : public HandlerOneClient {
    public:
        HandlerOneClientImpl(Client* client);

        void startHandler() override;

        ~HandlerOneClientImpl() override;

    private:
        void saveResultPollSet();
        void setPollSetBeforePoll();
        void handlingEvent();
        void deleteClient(std::list<Client*>::iterator* iterator);
        void deleteClientUser(Client* client);
        void deleteClientServer(Client* client);

        struct pollfd _pollSet[2];
        std::list<Client*> _clientList;

    };
}


#endif //LAB32PROXY_HANDLERONECLIENTIMPL_H