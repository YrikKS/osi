//
// Created by kurya on 02.11.2022.
//

#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H
//#include "ClientData.h"
#include "../buffer/Buffer.h"
#include "../binaryString/BinaryString.h"
#include <sys/poll.h>
#include <memory>

namespace ProxyServer {
    typedef enum {
        USER,
        HTTP_SERVER
    } TypeClient;

    class Client {
    public:
        virtual ~Client() = default;

        virtual int getFdClient() = 0;

        virtual void sendBuf(std::shared_ptr<std::string>* buf) = 0;

        virtual void readBuf(std::shared_ptr<std::string> *buf) = 0;

        virtual Buffer *getBuffer() = 0;

//        virtual Client *getPair() = 0;

        virtual void setBuffer(Buffer *buffer) = 0;

//        virtual void setPair(Client *pair) = 0;

        virtual TypeClient getTypeClient() = 0;

        virtual struct pollfd getPollFd() = 0;

        virtual void setPollElement(struct pollfd pollfd) = 0;

        virtual void setReventsZero() = 0;
    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
