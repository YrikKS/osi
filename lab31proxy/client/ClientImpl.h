//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CLIENTIMPL_H
#define LAB31PROXY_CLIENTIMPL_H

#include "Client.h"
#include "../logger/Logger.h"
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "ClientImpl.h"
#include "../Constants.h"
//#include "ClientDataImpl.h"

namespace ProxyServer {
    class ClientImpl : public Client {
    public:
        explicit ClientImpl(int sock, TypeClient typeClient, Buffer *buf);

        int getFdClient() override;

        void sendBuf(const char *buf) override;

        int readBuf(char *buf) override;

        Buffer *getBuffer() override;

        void setBuffer(Buffer *buffer) override;

        ~ClientImpl() override;

        TypeClient getTypeClient() override;

        void setPair(Client *pair) override;

        Client *getPair() override;

        struct pollfd getPollFd() override;

        void setPollElement(struct pollfd pollfd) override;

        void setReventsZero() override;

    private:
        int _fd;
        TypeClient _typeClient;
        ProxyServer::Buffer *_buffer;
        Client *_pair = NULL;
        struct pollfd _structPollFd;
    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
