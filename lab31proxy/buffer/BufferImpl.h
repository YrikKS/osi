//
// Created by kurya on 06.11.2022.
//

#ifndef LAB31PROXY_BUFFERIMPL_H
#define LAB31PROXY_BUFFERIMPL_H

#include <string>
#include "Buffer.h"
#include "../parser/ParserTypeRequest.h"
#include "../parser/ParserImpl.h"
#include "../Constants.h"
#include "../cash/Cash.h"
#include <string.h>

namespace ProxyServer {
    class BufferImpl : public Buffer {
    public:
        BufferImpl(Cash *cash);

        void readRequest(char *buf) override;

        void readResponse(char *buf) override;

        const char *sendBuf() override;

        void proofSend(const char *buf) override;

        bool isReadyConnectHttpServer() override;

        StatusHttp getStatusClient() override;

        StatusHttp getStatusHttpServer() override;

        bool isReadyToSend() override;

        void setStatusBuf(StatusHttp statusHttp) override;

        ResultParseHeading getParseResult() override;

        ~BufferImpl() override;

        bool isSendEnd() override;

    private:
        StatusHttp _statusClient = StatusHttp::WRITE_REQUEST_HEADING;
        StatusHttp _statusHttpServer = StatusHttp::READ_REQUEST;
        std::string _requestHeading;
        std::string _buf;
        std::string _sendingString;
        bool _isReadyToSend = false;
        bool _isEndSend = false;
        bool _isReadyConnectHttpServer = false;
        int _lengthBody = 0;
        ResultParseHeading *_resultParseHeading;
        void parsHead();
        bool checkCash();

        CashElement *_cashElement = NULL;
        bool _isGetDataFromCash = false;
        bool _isWrightDataToCash = false;
        bool error = false;
        int bytesReadFromCash = 0;
        Cash *_cash;
    };
}


#endif //LAB31PROXY_BUFFERIMPL_H
