//
// Created by kurya on 02.11.2022.
//

#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H
#include "ClientData.h"

namespace ProxyServer {
    typedef enum {
        USER,
        HTTP_SERVER
    } TypeClient;

    class Client {
    public:
        virtual ~Client() = default;

        virtual int getFdClient() = 0;

        virtual void sendBuf(const char *buf) = 0;

        virtual int readBuf(char *buf) = 0;

        virtual ClientData* getClientData() = 0;

        virtual Client* getPair() = 0;

        virtual void setPair(Client* pair) = 0;

        virtual TypeClient getTypeClient() = 0;
    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
