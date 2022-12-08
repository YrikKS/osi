//
// Created by kurya on 07.11.2022.
//


#include "CashElementImpl.h"

using namespace ProxyServer;

bool CashElementImpl::isCashEnd() {
    return _isCashEnd;
}

void CashElementImpl::setIsCashEnd(bool var) {
    _isCashEnd = var;
}

std::shared_ptr<std::string> CashElementImpl::getCash() {
    return _cash;
}

CashElementImpl::CashElementImpl(std::string heading) {
    _head = heading;
    std::hash<std::string> hasher;
    _hashHead = hasher(heading);
}

long long int CashElementImpl::getHash() {
    return _hashHead;
}

CashElementImpl::~CashElementImpl() {
    LOG_EVENT("delete cash ");
}

int CashElementImpl::getCountUsers() {
    return _countUsers;
}

void CashElementImpl::addCountUsers() {
    _countUsers++;
}

void CashElementImpl::minusCountUsers() {
    _countUsers--;
}

bool CashElementImpl::isIsServerConnected() {
    return _isServerConnected;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    _isServerConnected = isServerConnected;
}

const std::string &CashElementImpl::getHead() {
    return _head;
}

long long int CashElementImpl::getLength() {
    return _cash->size();
}

void CashElementImpl::memCopyFromCash(std::string *binaryString, long long int _countByteReadFromCash,
                                      long long int sizeCopy) {
    std::memcpy((void *) (binaryString)->c_str(), _cash->c_str() +
                                                  _countByteReadFromCash, sizeCopy);
}

void CashElementImpl::appendStringToCash(std::string *binaryString) {
    _cash->append(*binaryString);
}

void CashElementImpl::appendStringToCash(std::string binaryString) {
    _cash->append(binaryString);
}
