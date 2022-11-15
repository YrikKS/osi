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
#include <string.h>
#include <bitset>

namespace ProxyServer {
    class BufferImpl : public Buffer {
    public:
        BufferImpl(Cash *cash);

        void readFromSocket(BinaryString *binaryString) override;

//        void readResponse(char *buf) override;

        void sendBuf(BinaryString *binaryString) override;

        void proofSend(BinaryString *binaryString) override;

        bool isReadyConnectHttpServer() override;

        StatusHttp getStatusClient() override;

        StatusHttp getStatusHttpServer() override;

        bool isReadyToSend() override;

        void setStatusServer(StatusHttp statusHttp) override;

        void setStatusClient(StatusHttp statusHttp) override;

        ResultParseHeading getParseResult() override;

        ~BufferImpl() override;

        bool isSendEnd() override;

        bool isIsAddDataToCash() override;

        bool isIsDataGetCash() override;

        CashElement *getCashElement() override;

        bool isIsClientConnect() override;

        void setIsClientConnect(bool isClientConnect) override;

        bool isIsServerConnect() override;

        void setIsServerConnect(bool isServerConnect) override;

    private:
        StatusHttp _statusClient = StatusHttp::WRITE_REQUEST_HEADING;
        StatusHttp _statusHttpServer = StatusHttp::READ_REQUEST;
        std::string _requestHeading;
        BinaryString _buf;
        bool _isClientConnect = false;
        bool _isServerConnect = false;
        std::string _sendingString;
        bool _isReadyToSend = false;
        bool _isEndSend = false;
        bool _isReadyConnectHttpServer = false;
        long long int _lengthBody = 0;
        ResultParseHeading *_resultParseHeading = NULL;
        bool _isHaveContentLengthresponse = false;

        bool _isAddDataToCash = false;
        bool _isDataGetCash = false;
        size_t _countByteReadFromCash = 0;

        CashElement *_cashElement = NULL;

        void wrightRequestHeading(BinaryString *binaryString);

        void wrightRequestBody(BinaryString *binaryString);

        void wrightResponseHeading(BinaryString *binaryString);

        void wrightResponseBody(BinaryString *binaryString);

        void parsHead();

        bool error = false;
        Cash *_cash;

        bool isCashingData(int sizeHeading, ResultParseHeading resultParseHeading);
    };
}


#endif //LAB31PROXY_BUFFERIMPL_H
