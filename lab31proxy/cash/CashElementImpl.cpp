//
// Created by kurya on 07.11.2022.
//

#include "CashElementImpl.h"

using namespace ProxyServer;

bool CashElementImpl::isCashEnd() {
    return _isCashEnd;
}

void CashElementImpl::setIsCashEnd() {
    _isCashEnd = true;
}

std::string *CashElementImpl::getCash() {
    return &_cash;
}

CashElementImpl::CashElementImpl(std::string heading) {
    _head = heading;
    std::hash<std::string> hasher;
    _hashHead = hasher(heading);
}

size_t CashElementImpl::getHash() {
    return _hashHead;
}

CashElementImpl::~CashElementImpl() {
    LOG_EVENT("Delete cash");
    _cash.clear();
}
