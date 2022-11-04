//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CLIENTIMPL_H
#define LAB31PROXY_CLIENTIMPL_H

#include "Client.h"

namespace ProxyServer {
    class ClientImpl : public Client {
    public:
        explicit ClientImpl(int sock, STATUS_REQUEST statusRequest);

        int getFdClient() override;

        void sendBuf(char *buf) override;

        int readBuf(char *buf) override;

        ~ClientImpl();

        STATUS_REQUEST getStatusRequest() override;

        void setStatusRequest(STATUS_REQUEST statusRequest) override;

        std::string &getRequestHeading() override;

        void setRequestHeading(const std::string &requestHeading) override;

    private:
        int fd;
        STATUS_REQUEST statusRequest;
        std::string requestHeading;
    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
