//
// Created by kurya on 07.11.2022.
//

#include "CashImpl.h"
#include "CashElementImpl.h"
#include "../logger/Logger.h"

ProxyServer::CashElement *ProxyServer::CashImpl::findResponseInCash(std::string heading) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(heading);
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            LOG_EVENT("find in cash");
            return (*it);
        }
    }
    LOG_EVENT("not find in cash");
    return NULL;
}

ProxyServer::CashElement *ProxyServer::CashImpl::addStringToCash(std::string request) {
    if (_listCash.size() >= COUNT_CASH_ELEMENT) {
        if (_listCash.size() > 1) {
            delete *_listCash.begin();
            _listCash.pop_front();
        }
    }
    LOG_EVENT("add to cash");
    CashElementImpl *cash = new CashElementImpl(request);
    _listCash.push_back(cash);
    return cash;
}

ProxyServer::CashImpl::~CashImpl() {
    for (auto &it: _listCash) {
        delete it;
    }
}

