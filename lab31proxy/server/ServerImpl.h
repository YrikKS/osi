//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERIMPL_H
#define LAB31PROXY_SERVERIMPL_H


#include "Server.h"
#include "../logger/Logger.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <list>
#include "../Constants.h"
#include "../client/Client.h"
#include "../socket/ServerSocket.h"
#include "../socket/ServerSocketImpl.h"

namespace ProxyServer {
    class ServerImpl : public Server {
    public:
        ServerImpl();
        void startServer() override;
        ~ServerImpl();

    private:
        void setPollArr();
        void updatePollArr();
        void handlingEvent();
        bool deleteClient(Client* client, std::list<Client*>::iterator* iterator);

        bool _isWork = true;
        ServerSocket* _serverSocket;
        struct pollfd _pollSet[MAX_COUNT_CONNECTIONS];
        std::list<Client*> _clientList;
    };
}


#endif //LAB31PROXY_SERVERIMPL_H
