//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERSOCKETIMPL_H
#define LAB31PROXY_SERVERSOCKETIMPL_H

#include "ServerSocket.h"
#include "../logger/Logger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "MutexForServer.h"
#include "../Constants.h"
#include "../client/Client.h"
#include "../client/ClientImpl.h"
#include "../buffer/Buffer.h"
#include "../buffer/BufferImpl.h"

namespace ProxyServer {
    class ServerSocketImpl : public ServerSocket {
    public:
        ServerSocketImpl();

        int connectSocket() override;

        static Client *connectToClient(std::string url, int port);

        int getFdSocket() override;

        Client *acceptNewClient(Cash *cash) override;

        int acceptNewClientSock() override;

        void closeSocket() override;

        ~ServerSocketImpl();

        static pthread_mutex_t mutex;
    private:
        int serverSocket_ = 0;
    };
}


#endif //LAB31PROXY_SERVERSOCKETIMPL_H
