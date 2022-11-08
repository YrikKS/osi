//
// Created by kurya on 04.11.2022.
//

#include <iostream>
#include <regex>
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

    findHostAndPort(result, heading);
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

void ParserImpl::findHostAndPort(ResultParseHeading *result, std::string buf) {
    std::regex regex(REGEX_FOR_HOST_AND_PORT);
    std::smatch resRegex;

    if (regex_search(buf, resRegex, regex)) {
        if (resRegex.size() == 2) {
            result->setHostName(resRegex[1]);
        } else {
            result->setHostName(resRegex[1]);
            result->setPort(atoi(resRegex[2].str().substr(1).c_str()));
        }
//        for (auto it = resRegex.begin(); it != resRegex.end(); it++) {
//            std::cout << (*it) << std::endl;
//        }
    } else {
        result->setType(TypeRequest::INVALID_REQUEST);
        LOG_ERROR("incorrect heading");
        throw ParseException("incorrect heading");
    }
}
