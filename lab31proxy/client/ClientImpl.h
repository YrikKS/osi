//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CLIENTIMPL_H
#define LAB31PROXY_CLIENTIMPL_H

#include "Client.h"

namespace ProxyServer {
//    typedef enum {
//
//    } ;

    class ClientImpl : public Client {
    public:
        explicit ClientImpl(int sock);

        int getFdClient() override;

        void sendBuf(char *buf) override;

        int readBuf(char *buf) override;

        ~ClientImpl();

    private:
        int fd;

    };
}

#endif //LAB31PROXY_CLIENTIMPL_H
