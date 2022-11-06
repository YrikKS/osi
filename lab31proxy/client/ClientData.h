//
// Created by kurya on 05.11.2022.
//

#ifndef LAB31PROXY_CLIENTDATA_H
#define LAB31PROXY_CLIENTDATA_H

#include <string>

#include "../parser/ResultParseHeading.h"


namespace ProxyServer {
    typedef enum {
        WRITE_REQUEST_HEADING,
        WRITE_REQUEST_BODY,
        READ_RESPONSE,
        READ_REQUEST,
        WRITE_RESPONSE
        //WRITE_REQUEST
    } StatusHttp;

    class ClientData {
    public:
        virtual ~ClientData() = default;

        virtual StatusHttp getStatusRequest() = 0;

        virtual void setStatusRequest(StatusHttp statusRequest) = 0;

        virtual std::string &getRequestHeading() = 0;

        virtual void setRequestHeading(const std::string &basicString) = 0;

        virtual void setResultParseHeading(ResultParseHeading *parseHeading) = 0;

        virtual ResultParseHeading *getResultParseHeading() = 0;

        virtual const std::string &getRequestBody() = 0;

        virtual void setRequestBody(const std::string &requestBody) = 0;

        virtual bool getIsReadyToSend() = 0;

        virtual void setIsReadyToSend(bool isReadyToSend) = 0;

        virtual void addToRequestBody(std::string string) = 0;

        virtual void addToRequestHeading(std::string str) = 0;

        virtual bool isEndRequestOrResponse() = 0;

        virtual void setEndRequestOrResponse(bool endRequestOrResponse) = 0;
    };
}

#endif //LAB31PROXY_CLIENTDATA_H
