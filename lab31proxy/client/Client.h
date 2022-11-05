//
// Created by kurya on 02.11.2022.
//

#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H
#include "ClientData.h"

namespace ProxyServer {
    class Client {
    public:
        virtual ~Client() = default;

        virtual int getFdClient() = 0;

        virtual void sendBuf(const char *buf) = 0;

        virtual int readBuf(char *buf) = 0;

        virtual ClientData* getClientData() = 0;
    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
