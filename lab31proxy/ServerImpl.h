//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERIMPL_H
#define LAB31PROXY_SERVERIMPL_H


#include "Server.h"
#include "logger/Logger.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <list>
#include "Constants.h"
#include "client/Client.h"
#include "socket/ServerSocket.h"
#include "socket/ServerSocketImpl.h"

namespace ProxyServer {
    class ServerImpl : public Server {
    public:
        ServerImpl();
        void startServer() override;
        ~ServerImpl() override ;

    private:
        void updatePollFd();
        void handlingEvent();
        void handlingReadBuf(char* buf, Client* client);
        bool _isWork = true;
        ServerSocket* _serverSocket;
        struct pollfd _pollSet[MAX_COUNT_CONNECTIONS];
        std::list<Client*> _clientList;
    };
}


#endif //LAB31PROXY_SERVERIMPL_H
