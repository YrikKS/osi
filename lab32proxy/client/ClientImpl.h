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

        int sendBuf(std::string *buf) override;

        int readBuf(std::string *buf) override;

        Buffer *getBuffer() override;

        void setBuffer(Buffer *buffer) override;

        ~ClientImpl() override;

        TypeClient getTypeClient() override;

        void setEvents(int event) override;

        struct pollfd getPollFd() override;

        void setPollElement(struct pollfd pollfd) override;

        void setReventsZero() override;

        bool isInClientList() override;

        void setInClientList(bool value) override;

        Client *getPair();

        void setPair(Client *pair);

        std::list<Client*> getListHandlingEvent() override;

        void addClientToHandlingEvent(Client* client) override;

        void eraseIt(Client* client) override;

    private:
        bool _isInClientList = true;
        int _fd;
        TypeClient _typeClient;
        ProxyServer::Buffer *_buffer;
        std::list<Client*> _listHandlingEvent;
        Client *_pair = NULL;
        struct pollfd _structPollFd;
    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
