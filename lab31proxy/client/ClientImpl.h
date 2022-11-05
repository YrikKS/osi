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
#include "ClientDataImpl.h"

namespace ProxyServer {
    class ClientImpl : public Client {
    public:
        explicit ClientImpl(int sock, STATUS_REQUEST statusRequest);

        int getFdClient() override;

        void sendBuf(const char *buf) override;

        int readBuf(char *buf) override;

        ClientData* getClientData() override;

        ~ClientImpl() override;
    private:
        int fd;
        ClientData* clientData;
    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
