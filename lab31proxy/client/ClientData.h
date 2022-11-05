//
// Created by kurya on 05.11.2022.
//

#ifndef LAB31PROXY_CLIENTDATA_H
#define LAB31PROXY_CLIENTDATA_H

#include <string>

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

    class ClientData {
    public:
        virtual ~ClientData() = default;

        virtual STATUS_REQUEST getStatusRequest() = 0;

        virtual void setStatusRequest(STATUS_REQUEST statusRequest) = 0;

        virtual std::string &getRequestHeading() = 0;

        virtual void setRequestHeading(const std::string &basicString) = 0;

        virtual void setResultParseHeading(ResultParseHeading *parseHeading) = 0;

        virtual ResultParseHeading *getResultParseHeading() = 0;

        virtual const std::string &getRequestBody() = 0;

        virtual void setRequestBody(const std::string &requestBody) = 0;
    };
}

#endif //LAB31PROXY_CLIENTDATA_H
