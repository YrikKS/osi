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

        ClientDataImpl(STATUS_REQUEST statusRequest);

        STATUS_REQUEST getStatusRequest() override;

        std::string &getRequestHeading() override;

        void setStatusRequest(STATUS_REQUEST statusRequest) override;

        void setRequestHeading(const std::string &basicString) override;

        void setResultParseHeading(ResultParseHeading *parseHeading) override;

        ResultParseHeading *getResultParseHeading() override;

        const std::string &getRequestBody() override;

        void setRequestBody(const std::string &requestBody) override;

    private:
        STATUS_REQUEST statusRequest;
        std::string requestHeading;
        std::string requestBody;
        ResultParseHeading *resultParseHeading;
    };

}


#endif //LAB31PROXY_CLIENTDATAIMPL_H
