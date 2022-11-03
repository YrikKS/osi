//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERIMPL_H
#define LAB31PROXY_SERVERIMPL_H


#include "Server.h"
#include <poll.h>
#include <iostream>
#include <map>
#include <list>
#include "Constants.h"
#include "Client/Client.h"
#include "Socket/ServerSocket.h"
#include "Socket/ServerSocketImpl.h"

namespace ProxyServer {
    class ServerImpl : public Server {
    public:
        ServerImpl();
        void startServer() override;


    private:
        void updatePollFd();
        void handlingEvent();
        bool _isWork = true;
        ServerSocket* _serverSocket;
        struct pollfd _pollSet[MAX_COUNT_CONNECTIONS];
        std::list<Client*> _clientList;
    };
}


#endif //LAB31PROXY_SERVERIMPL_H
