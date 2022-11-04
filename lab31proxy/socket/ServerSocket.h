//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERSOCKET_H
#define LAB31PROXY_SERVERSOCKET_H

#include "../client/Client.h"

namespace ProxyServer {
    class ServerSocket {
    public:
        virtual int connectSocket() = 0;

        virtual int getFdSocket() = 0;

        virtual Client* acceptNewClient() = 0;

        virtual void closeSocket() = 0;
    };
}
#endif //LAB31PROXY_SERVERSOCKET_H
