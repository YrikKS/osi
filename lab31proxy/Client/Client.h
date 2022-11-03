//
// Created by kurya on 02.11.2022.
//

#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H

namespace ProxyServer {
    class Client {
    public:
        virtual void sendResponse() = 0;

        virtual int getFdClient() = 0;

        virtual void sendBuf(char *buf) = 0;

        virtual char *readBuf() = 0;

    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
