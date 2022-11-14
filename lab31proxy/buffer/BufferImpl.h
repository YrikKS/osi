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
#include <bitset>

namespace ProxyServer {
    class BufferImpl : public Buffer {
    public:
        BufferImpl(Cash *cash);

        void readFromSocket(BinaryString* binaryString) override;

//        void readResponse(char *buf) override;

        void sendBuf(BinaryString* binaryString) override;

        void proofSend(BinaryString* binaryString) override;

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
        BinaryString _buf;
        std::bitset<8> _bitBuffer;
        std::string _sendingString;
        bool _isReadyToSend = false;
        bool _isEndSend = false;
        bool _isReadyConnectHttpServer = false;
        int _lengthBody = 0;
        ResultParseHeading *_resultParseHeading = NULL;
        bool _isHaveContentLengthresponse = false;
        void parsHead();
        bool checkCash();
        bool isCashingData(ResultParseHeading resultParseHeading);


        CashElement *_cashElement = NULL;
        bool _isGetDataFromCash = false;
        bool _isWrightDataToCash = false;
        bool error = false;
        int bytesReadFromCash = 0;
        Cash *_cash;
    };
}


#endif //LAB31PROXY_BUFFERIMPL_H
