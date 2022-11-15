//
// Created by kurya on 06.11.2022.
//

#ifndef LAB31PROXY_BUFFER_H
#define LAB31PROXY_BUFFER_H

#include "../parser/ResultParseHeading.h"
#include "../binaryString/BinaryString.h"
#include "../cash/CashElement.h"
#include "../cash/Cash.h"

namespace ProxyServer {
    typedef enum {
        WRITE_REQUEST_HEADING,
        WRITE_REQUEST_BODY,
        READ_RESPONSE,
        READ_REQUEST,
        WRITE_RESPONSE_HEADING,
        WRITE_RESPONSE_BODY,
        END_WORK
        //WRITE_REQUEST
    } StatusHttp;

    class Buffer {
    public:
        virtual void readFromSocket(BinaryString *binaryString) = 0;

//        virtual void readResponse(char *buf) = 0;

        virtual void sendBuf(BinaryString *binaryString) = 0;

        virtual void proofSend(BinaryString *binaryString) = 0;

        virtual bool isReadyConnectHttpServer() = 0;

        virtual ResultParseHeading getParseResult() = 0;

        virtual StatusHttp getStatusClient() = 0;

        virtual StatusHttp getStatusHttpServer() = 0;

        virtual void setStatusServer(StatusHttp statusHttp) = 0;

        virtual void setStatusClient(StatusHttp statusHttp) = 0;

        virtual bool isReadyToSend() = 0;

        virtual ~Buffer() = default;

        virtual bool isSendEnd() = 0;

        virtual bool isIsAddDataToCash() = 0;

        virtual bool isIsDataGetCash() = 0;

        virtual CashElement *getCashElement() = 0;
    };

}


#endif //LAB31PROXY_BUFFER_H
