//
// Created by kurya on 05.11.2022.
//

#include "ClientDataImpl.h"

ProxyServer::StatusHttp ProxyServer::ClientDataImpl::getStatusRequest() {
    return _statusRequest;
}

void ProxyServer::ClientDataImpl::setStatusRequest(ProxyServer::StatusHttp statusRequest) {
    ClientDataImpl::_statusRequest = statusRequest;
}

void ProxyServer::ClientDataImpl::setRequestHeading(const std::string &basicString) {
    ClientDataImpl::_requestHeading = basicString;
}

std::string& ProxyServer::ClientDataImpl::getRequestHeading() {
    return _requestHeading;
}

void ProxyServer::ClientDataImpl::setResultParseHeading(ProxyServer::ResultParseHeading *parseHeading) {
    _resultParseHeading = parseHeading;
}

ProxyServer::ResultParseHeading *ProxyServer::ClientDataImpl::getResultParseHeading() {
    return _resultParseHeading;
}

ProxyServer::ClientDataImpl::ClientDataImpl(StatusHttp statusRequest) {
    this->_statusRequest = statusRequest;
    _resultParseHeading = NULL;
}

ProxyServer::ClientDataImpl::~ClientDataImpl() {
    if(_resultParseHeading != NULL) {
        delete _resultParseHeading;
    }
}

const std::string &ProxyServer::ClientDataImpl::getRequestBody() {
    return _requestBody;
}

void ProxyServer::ClientDataImpl::setRequestBody(const std::string &requestBody) {
    ClientDataImpl::_requestBody = requestBody;
}

bool ProxyServer::ClientDataImpl::getIsReadyToSend() {
    return _isReadyToSend;
}

void ProxyServer::ClientDataImpl::setIsReadyToSend(bool isReadyToSend) {
    ClientDataImpl::_isReadyToSend = isReadyToSend;
}

void ProxyServer::ClientDataImpl::addToRequestBody(std::string string) {
    _requestBody += string;
}


