//
// Created by kurya on 04.11.2022.
//

#include <iostream>
#include "ParserImpl.h"

ProxyServer::TypeRequest ProxyServer::ParserImpl::parsingRequest(char *buf) {
    std::cout << "parsing request..." << std::endl;
    return ProxyServer::NOT_GET_REQUEST;
}
