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

ProxyServer::ResultParseHeading *ProxyServer::ParserImpl::parsingHeading(std::string heading) {
    ResultParseHeading *result = new ResultParseHeading;
    if (heading.size() > 3) {
        if (heading.substr(0, 3) == "GET") {
            result->setType(TypeRequest::GET_REQUEST);
        } else {
            result->setType(TypeRequest::NOT_GET_REQUEST);
        }
    } else {
        result->setType(TypeRequest::INVAILD_REQUEST);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
    }

    int contentLength = heading.find("Content-Length: ");
    if (contentLength != std::string::npos) {
        int endContentLength = heading.find("\r\n", contentLength);
        if (endContentLength != std::string::npos) {
            contentLength += std::string("Content-Length: ").size();
            result->setContentLength(atoi(
                    heading.substr(contentLength, endContentLength - contentLength).c_str()));
        }
    } else {
        if(result->getType() == TypeRequest::GET_REQUEST) {
            result->setType(TypeRequest::GET_REQUEST_NOT_CASH);
        }
        result->setContentLength(-1);
    }

    int host = heading.find("Host: ");
    if (host != std::string::npos) {
        int endContentLength = heading.find("\r\n", host);
        if (endContentLength != std::string::npos) {
            host += std::string("Host: ").size();
            result->setHostName(heading.substr(host, endContentLength - host));
        } else {
            host += std::string("Host: ").size();
            result->setHostName(heading.substr(host));
        }
    } else {
        result->setType(TypeRequest::INVAILD_REQUEST);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
    }

    return result;
}
