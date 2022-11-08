//
// Created by kurya on 04.11.2022.
//

#include <iostream>
#include "ParserImpl.h"

using namespace ProxyServer;

ResultPars ParserImpl::findEndHeading(std::string buf, int *posEnd) {
    int resultFinding = buf.find("\r\n\r\n");
    if (resultFinding == std::string::npos) {
//        resultFinding = buf.find("\n\n");
//        if (resultFinding != std::string::npos) {
//            *posEnd = resultFinding + 2;
//            return ResultPars::END_HEADING;
//        }
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding + 4;
        return ResultPars::END_HEADING;
    }
}

TypeRequest ParserImpl::parsingRequest(char *buf, char *host) {
    return NOT_GET_REQUEST;
}

ResultParseHeading *ParserImpl::parsingHeading(std::string heading) {
    ResultParseHeading *result = new ResultParseHeading;
    if (heading.size() > 3) {
        if (heading.substr(0, 3) == "GET") {
            result->setType(TypeRequest::GET_REQUEST);
        } else {
            result->setType(TypeRequest::NOT_GET_REQUEST);
        }
    } else {
        result->setType(TypeRequest::INVALID_REQUEST);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
        // TODO обработать ?
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
//        if (result->getType() == TypeRequest::GET_REQUEST) {
//            result->setType(TypeRequest::GET_REQUEST_NOT_CASH);
//        }
        result->setContentLength(0);
    }

    int host = heading.find("Host: ");
    if (host != std::string::npos) {
        int findPort = heading.find(":", host);
        if (findPort != std::string::npos) {
            int endContentLength = heading.find("\r\n", host);
            if (endContentLength != std::string::npos) {
                host += std::string("Host: ").size();
                findPort += 1;
                result->setHostName(heading.substr(host, findPort - host));
                result->setPort(atoi(heading.substr(findPort, endContentLength - findPort).c_str()));
            }
        } else {
            int endContentLength = heading.find("\r\n", host);
            if (endContentLength != std::string::npos) {
                host += std::string("Host: ").size();
                result->setHostName(heading.substr(host, endContentLength - host));
            }
        }
    } else {
        result->setType(TypeRequest::INVALID_REQUEST);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
    }
    std::cout << result->getHostName() << " " << result->getPort() << std::endl;
    return result;
}

ResultParseHeading ParserImpl::parsingResponseHeading(std::string heading) {
    ResultParseHeading result; //HTTP/1.1 200 OK
    int statusResponse = heading.find("HTTP/1.1 200 OK");
    if (statusResponse == std::string::npos) {
        result.setResponseWithError(true);
    } else {
        result.setResponseWithError(false);
    }

    int contentLength = heading.find("Content-Length: ");
    if (contentLength != std::string::npos) {
        int endContentLength = heading.find("\r\n", contentLength);
        if (endContentLength != std::string::npos) {
            contentLength += std::string("Content-Length: ").size();
            result.setContentLength(atoi(
                    heading.substr(contentLength, endContentLength - contentLength).c_str()));
        }
    } else { // TODO rework
        if (result.getType() == TypeRequest::GET_REQUEST) {
            result.setType(TypeRequest::GET_REQUEST_NOT_CASH);
        }
        result.setContentLength(-1);
    }

    return result;
}

ResultPars ParserImpl::findEndBody(std::string buffer, int *posEnd) {
    int resultFinding = buffer.find("0\r\n\r\n");
    if (resultFinding == std::string::npos) {
//        resultFinding = buffer.find("0\n\n");
//        if (resultFinding != std::string::npos) {
//            *posEnd = resultFinding + 3;
//            return ResultPars::END_HEADING;
//        }
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding + 5;
        return ResultPars::END_BODY;
    }
}
