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

namespace ProxyServer {
    class BufferImpl : public Buffer {
    public:
        void readRequest(char *buf) override;

        void readResponse(char *buf) override;

        char *sendBuf() override;

        void proofSend(char *buf) override;

        bool isReadyConnectHttpServer() override;

        StatusHttp getStatusBuf() override;

        bool isReadyToSend() override;

        void setStatusBuf(StatusHttp statusHttp) override;

        ResultParseHeading getParseResult() override;

        ~BufferImpl() override;

        bool isSendEnd() override;

    private:
        StatusHttp _statusClient;
        StatusHttp _statusHttpServer;
        std::string _requestHeading;
        std::string _buf;
        std::string _sendingString;
        bool _isReadyToSend = false;
        bool _isEndSend = false;
        bool _isReadyConnectHttpServer = false;
        int _lengthBody = 0;
        ResultParseHeading *_resultParseHeading;
    };
}


#endif //LAB31PROXY_BUFFERIMPL_H
