//
// Created by kurya on 06.12.2022.
//

#ifndef LAB31PROXY_NEWSERVERIMPL_H
#define LAB31PROXY_NEWSERVERIMPL_H


#include "Server.h"
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
    class NewServerImpl : public Server {
    public:
        NewServerImpl();
        void startServer() override;
        ~NewServerImpl();

    private:
        void saveResultPollSet();
        void setPollSetBeforePoll();
        void handlingEvent();
        void deleteClient(std::list<Client*>::iterator* iterator);
        void deleteClientUser(Client* client);
        void deleteClientServer(Client* client);
        void setPollElements();
        void changePollEventForClient(Client* client);


        ServerSocket* _serverSocket;
        struct pollfd _pollSet[MAX_COUNT_CONNECTIONS];
        std::list<Client*> _clientList;
        Cash *_cash;
    };
}

#endif //LAB31PROXY_NEWSERVERIMPL_H
