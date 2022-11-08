//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_RESULTPARSEHEADING_H
#define LAB31PROXY_RESULTPARSEHEADING_H

#include <string>
#include "ParserTypeRequest.h"

namespace ProxyServer {
    class ResultParseHeading {
    public:
        const std::string &getHostName() const {
            return hostName;
        }

        void setHostName(const std::string &hostName) {
            ResultParseHeading::hostName = hostName;
        }

        int getContentLength() const {
            return contentLength;
        }

        void setContentLength(int contentLength) {
            ResultParseHeading::contentLength = contentLength;
        }

        TypeRequestAndResponse getType() const {
            return type;
        }

        void setType(TypeRequestAndResponse type) {
            ResultParseHeading::type = type;
        }

        ~ResultParseHeading() {

        }

        bool isResponseWithError() {
            return responseWithError;
        }

        void setResponseWithError(bool responseWithError) {
            ResultParseHeading::responseWithError = responseWithError;
        }

        int getPort() {
            return port;
        }

        void setPort(int port) {
            ResultParseHeading::port = port;
        }

    private:
        std::string hostName;
        int contentLength = 0;
        TypeRequestAndResponse type;
        bool responseWithError = false;
        int port = 80;

    };

}
#endif //LAB31PROXY_RESULTPARSEHEADING_H
