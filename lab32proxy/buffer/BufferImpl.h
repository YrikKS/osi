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

        void readFromSocket(std::string *binaryString) override;

        void sendBuf(std::string *binaryString) override;

        void proofSend(std::string *binaryString) override;

        bool isReadyConnectHttpServer() override;

        void setReadyConnectHttpServer(bool value) override;

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
        std::shared_ptr<std::string> _buf = std::make_shared<std::string>();
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
        long long int _countByteReadFromCash = 0;

        CashElement *_cashElement = NULL;

        void wrightRequestHeading(std::string *binaryString);

        void wrightRequestBody(std::string *binaryString);

        void wrightResponseHeading(std::string *binaryString);

        void wrightResponseBody(std::string *binaryString);

        void parsHead();

        bool error = false;
        Cash *_cash;

        bool isCashingData(int sizeHeading, ResultParseHeading resultParseHeading);
    };
}


#endif //LAB31PROXY_BUFFERIMPL_H
