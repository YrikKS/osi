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

ProxyServer::CashElement *ProxyServer::CashImpl::addStringToCash(std::string request, long long int dataSize) {
    dataSize += request.length();
    if (byteInCash + dataSize >= MAX_CASH_SIZE) {
        if (dataSize > SIZE_EACH_CASH_ELEMENT) {
            return NULL;
        }
        for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
            if ((*it)->getCountUsers() <= 0) {
                byteInCash -= (*it)->getCash()->length();
                byteInCash -= (*it)->getHead().length();
                it = _listCash.erase(it);
                if (byteInCash + dataSize < MAX_CASH_SIZE) {
                    break;
                }
            }
        }
        if (byteInCash + dataSize >= MAX_CASH_SIZE) {
            return NULL;
        }
    }

    LOG_EVENT("add to cash");
    byteInCash += dataSize;
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

