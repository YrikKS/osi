//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERSOCKET_H
#define LAB31PROXY_SERVERSOCKET_H

#include "../client/Client.h"
#include "ConnectException.h"
#include "../parser/ParserImpl.h"
#include "../cash/Cash.h"

namespace ProxyServer {
    class ServerSocket {
    public:
        virtual int connectSocket() = 0;

        virtual int getFdSocket() = 0;

        virtual Client* acceptNewClient(Cash* cash) = 0;

        virtual int acceptNewClientSock() = 0;
//        virtual Client* connectToClient(std::string url, int port) = 0;

        virtual void closeSocket() = 0;
    };
}
#endif //LAB31PROXY_SERVERSOCKET_H
