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
            _listCash.splice(_listCash.end(), _listCash, it);
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
//            std::cout << "need del" << std::endl;
            bool isDel = false;
            for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
                if ((*it)->getCountUsers() <= 0) {
                    _listCash.erase(it);
                    isDel = true;
                    break;
                }
            }
            if (!isDel) {
                return NULL;
            }
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

bool ProxyServer::CashImpl::isElementInCash(std::string request) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(request);
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            return true;
        }
    }
    return false;
}

