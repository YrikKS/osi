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

        TypeRequest getType() const {
            return type;
        }

        void setType(TypeRequest type) {
            ResultParseHeading::type = type;
        }

    public:
    private:
        std::string hostName;
        int contentLength;
        TypeRequest type;
    };
}
#endif //LAB31PROXY_RESULTPARSEHEADING_H
