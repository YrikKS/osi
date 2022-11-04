//
// Created by kurya on 02.11.2022.
//

#ifndef LAB31PROXY_CLIENTINTERFACE_H
#define LAB31PROXY_CLIENTINTERFACE_H

#include "../parser/ResultParseHeading.h"

namespace ProxyServer {
    typedef enum {
        READ_REQUEST_HEADING,
        READ_REQUEST_BODY,
        READ_RESPONESE_HEADING,
        READ_RESPONESE_BODY,
        WRITE_RESPONSE,
        WRITE_REQUEST
    } STATUS_REQUEST;

    class Client {
    public:
        virtual int getFdClient() = 0;

        virtual void sendBuf(char *buf) = 0;

        virtual int readBuf(char *buf) = 0;

        virtual STATUS_REQUEST getStatusRequest() = 0;

        virtual void setStatusRequest(STATUS_REQUEST statusRequest) = 0;

        virtual std::string &getRequestHeading() = 0;

        virtual void setRequestHeading(const std::string &requestHeading) = 0;

        virtual void setResultParseHeading(ResultParseHeading *parseHeading) = 0;

        virtual ResultParseHeading* getResultParseHeading() = 0;
    };
}
#endif //LAB31PROXY_CLIENTINTERFACE_H
