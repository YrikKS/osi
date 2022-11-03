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

namespace ProxyServer {
    class ServerImpl : public Server {
    public:
        void startServer() override;

    private:
        bool isWork = true;
        int countClient = 0;
        struct pollfd *pollSet;
        std::list<Client> clientList;
    };
}


#endif //LAB31PROXY_SERVERIMPL_H
