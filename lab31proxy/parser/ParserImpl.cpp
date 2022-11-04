//
// Created by kurya on 04.11.2022.
//

#include <iostream>
#include "ParserImpl.h"

ProxyServer::ResultPars ProxyServer::ParserImpl::pars(char *buf, int *posEnd) {
    std::string buffer(buf);
    int resultFinding = buffer.find("\r\n\r\n");
    if (resultFinding == std::string::npos) {
        *posEnd = -1;
        return ResultPars::NOTHING;
    } else {
        *posEnd = resultFinding;
        return ResultPars::END_REQUEST_HEADING;
    }
}

ProxyServer::TypeRequest ProxyServer::ParserImpl::parsingRequest(char *buf, char *host) {

    return ProxyServer::NOT_GET_REQUEST;
}
