//
// Created by kurya on 04.11.2022.
//

#include <iostream>
#include "ParserImpl.h"

ProxyServer::ResultPars ProxyServer::ParserImpl::findEndHeading(char *buf, int *posEnd) {
    std::string buffer(buf);
    int resultFinding = buffer.find("\r\n\r\n");
    if (resultFinding == std::string::npos) {
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding + 4;
        return ResultPars::END_REQUEST_HEADING;
    }
}

ProxyServer::TypeRequest ProxyServer::ParserImpl::parsingRequest(char *buf, char *host) {

    return ProxyServer::NOT_GET_REQUEST;
}

ProxyServer::ResultParseHeading ProxyServer::ParserImpl::parsingHeading(std::string heading) {
//    if(heading.substr(0, 3))
    std::cout << heading.substr(0, 3) << std::endl;
    int contentLength = heading.find("Content-Length: ");
    if(contentLength != std::string::npos) {
        int endContentLength = heading.find("\n", contentLength);
        std::cout << heading.substr(contentLength, endContentLength - contentLength) << std::endl;
    }
    int host = heading.find("Host: ");
    if(host != std::string::npos) {
        int endContentLength = heading.find("\n", contentLength);
        std::cout << heading.substr(host, endContentLength - host) << std::endl;
    }

    std::cout << "end parse" << std::endl;
}
