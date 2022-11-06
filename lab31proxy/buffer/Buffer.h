//
// Created by kurya on 06.11.2022.
//

#ifndef LAB31PROXY_BUFFER_H
#define LAB31PROXY_BUFFER_H

#include "../parser/ResultParseHeading.h"

namespace ProxyServer {
    typedef enum {
        WRITE_REQUEST_HEADING,
        WRITE_REQUEST_BODY,
        READ_RESPONSE,
        READ_REQUEST,
        WRITE_RESPONSE,
        END_WORK
        //WRITE_REQUEST
    } StatusHttp;

    class Buffer {
    public:
        virtual void readRequest(char *buf) = 0;

        virtual void readResponse(char *buf) = 0;

        virtual const char *sendBuf() = 0;

        virtual void proofSend(const char *buf) = 0;

        virtual bool isReadyConnectHttpServer() = 0;

        virtual ResultParseHeading getParseResult() = 0;

        virtual StatusHttp getStatusBuf() = 0;

        virtual void setStatusBuf(StatusHttp statusHttp) = 0;

        virtual bool isReadyToSend() = 0;

        virtual ~Buffer() = default;

        virtual bool isSendEnd() = 0;
    };

}


#endif //LAB31PROXY_BUFFER_H
