//
// Created by kurya on 05.11.2022.
//

#include "ClientDataImpl.h"

ProxyServer::STATUS_REQUEST ProxyServer::ClientDataImpl::getStatusRequest() {
    return statusRequest;
}

void ProxyServer::ClientDataImpl::setStatusRequest(ProxyServer::STATUS_REQUEST statusRequest) {
    ClientDataImpl::statusRequest = statusRequest;
}

void ProxyServer::ClientDataImpl::setRequestHeading(const std::string &basicString) {
    ClientDataImpl::requestHeading = basicString;
}

std::string ProxyServer::ClientDataImpl::getRequestHeading() {
    return requestHeading;
}

void ProxyServer::ClientDataImpl::setResultParseHeading(ProxyServer::ResultParseHeading *parseHeading) {
    resultParseHeading = parseHeading;
}

ProxyServer::ResultParseHeading *ProxyServer::ClientDataImpl::getResultParseHeading() {
    return resultParseHeading;
}

ProxyServer::ClientDataImpl::ClientDataImpl(STATUS_REQUEST statusRequest) {
    this->statusRequest = statusRequest;
    resultParseHeading = NULL;
}

ProxyServer::ClientDataImpl::~ClientDataImpl() {
    if(resultParseHeading != NULL) {
        delete resultParseHeading;
    }
}

const std::string &ProxyServer::ClientDataImpl::getRequestBody() {
    return requestBody;
}

void ProxyServer::ClientDataImpl::setRequestBody(const std::string &requestBody) {
    ClientDataImpl::requestBody = requestBody;
}


