//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERSOCKETIMPL_H
#define LAB31PROXY_SERVERSOCKETIMPL_H

#include "ServerSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include "../Constants.h"
#include "../Client/Client.h"
#include "../Client/ClientImpl.h"

namespace ProxyServer {
    class ServerSocketImpl : public ServerSocket{
    public:
        int connectSocket() override;

        int getFdSocket() override;

        Client* acceptNewClient() override;

        void closeSocket() override;

        ~ServerSocketImpl();

    private:
        int serverSocket_;
    };
}


#endif //LAB31PROXY_SERVERSOCKETIMPL_H
