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
//#include "ClientImpl.h"
#include "../Constants.h"

namespace ProxyServer {
    class ClientImpl : public Client {
    public:
        explicit ClientImpl(int sock, TypeClient typeClient, Buffer *buf);

        int getFdClient() override;

        void sendBuf(std::string *buf) override;

        void readBuf(std::string *buf) override;

        Buffer *getBuffer() override;

        void setBuffer(Buffer *buffer) override;

        ~ClientImpl() override;

        TypeClient getTypeClient() override;

        Client *getPair();

        void setPair(Client *pair);

        std::list<Client*> getListHandlingEvent() override;

        void addClientToHandlingEvent(Client* client) override;

        void eraseIt(Client* client) override;

    private:
        int _fd;
        TypeClient _typeClient;
        ProxyServer::Buffer *_buffer;
        std::list<Client*> _listHandlingEvent;
        Client *_pair = NULL;
    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
