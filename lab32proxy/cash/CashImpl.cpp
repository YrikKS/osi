//
// Created by kurya on 07.11.2022.
//

#include "CashImpl.h"
#include "CashElementImpl.h"
#include "../logger/Logger.h"

ProxyServer::CashElement *ProxyServer::CashImpl::findResponseInCash(std::string heading) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(heading);
    pthread_mutex_lock(&mutex);
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            _listCash.splice(_listCash.end(), _listCash, it);
            LOG_EVENT("find in cash");
            CashElement* local = (*it);
            pthread_mutex_unlock(&mutex);
            return local;
        }
    }
    pthread_mutex_unlock(&mutex);
    LOG_EVENT("not find in cash");
    return NULL;
}

ProxyServer::CashElement *ProxyServer::CashImpl::addStringToCash(std::string request, long long int dataSize) {
    dataSize += request.length();
    pthread_mutex_lock(&mutex);
    if (dataSize > SIZE_EACH_CASH_ELEMENT) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    if (byteInCash + dataSize >= MAX_CASH_SIZE) {
        for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
            if ((*it)->getCountUsers() <= 0) {
                byteInCash -= (*it)->getLength();
                byteInCash -= (*it)->getHeading().length();
                it = _listCash.erase(it);
                if (byteInCash + dataSize < MAX_CASH_SIZE) {
                    break;
                }
            }
        }
        if (byteInCash + dataSize >= MAX_CASH_SIZE) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }
    LOG_EVENT("add to cash");
    byteInCash += dataSize;
    CashElementImpl *cash = new CashElementImpl(request, dataSize);
    _listCash.push_back(cash);
    pthread_mutex_unlock(&mutex);
    return cash;
}

ProxyServer::CashImpl::~CashImpl() {
    for (auto &it: _listCash) {
        delete it;
    }
    pthread_mutex_destroy(&mutex);
}

bool ProxyServer::CashImpl::isElementInCash(std::string request) {
    std::hash<std::string> hasher;
    size_t hashHeading = hasher(request);
    pthread_mutex_lock(&mutex);
    for (auto it = _listCash.begin(); it != _listCash.end(); it++) {
        if ((*it)->getHash() == hashHeading) {
            pthread_mutex_unlock(&mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

ProxyServer::CashImpl::CashImpl() {
    pthread_mutex_init(&mutex, NULL);
}

