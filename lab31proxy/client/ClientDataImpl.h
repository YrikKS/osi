//
// Created by kurya on 05.11.2022.
//

#ifndef LAB31PROXY_CLIENTDATAIMPL_H
#define LAB31PROXY_CLIENTDATAIMPL_H

#include "ClientData.h"
#include "Client.h"

namespace ProxyServer {
    class ClientDataImpl : public ClientData {
    public:
        ~ClientDataImpl() override;

        ClientDataImpl(StatusHttp statusRequest);

        StatusHttp getStatusRequest() override;

        std::string &getRequestHeading() override;

        void setStatusRequest(StatusHttp statusRequest) override;

        void setRequestHeading(const std::string &basicString) override;

        void setResultParseHeading(ResultParseHeading *parseHeading) override;

        ResultParseHeading *getResultParseHeading() override;

        const std::string &getRequestBody() override;

        void setRequestBody(const std::string &requestBody) override;

        bool getIsReadyToSend() override;

        void setIsReadyToSend(bool isReadyToSend) override;

        void addToRequestBody(std::string string) override;
    private:
        StatusHttp _statusRequest;
        std::string _requestHeading;
        std::string _requestBody;
        bool _isReadyToSend = false;
        ResultParseHeading *_resultParseHeading;
    };

}


#endif //LAB31PROXY_CLIENTDATAIMPL_H
