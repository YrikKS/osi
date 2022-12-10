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
    pthread_mutex_init(&mutex, NULL);
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
    pthread_mutex_lock(&mutex);
    int local = _countUsers;
    pthread_mutex_unlock(&mutex);
    return local;
}

void CashElementImpl::addCountUsers() {
    pthread_mutex_lock(&mutex);
    _countUsers++;
    pthread_mutex_unlock(&mutex);
}

void CashElementImpl::minusCountUsers() {
    pthread_mutex_lock(&mutex);
    _countUsers--;
    pthread_mutex_unlock(&mutex);
}

bool CashElementImpl::isIsServerConnected() {
    pthread_mutex_lock(&mutex);
    size_t local = _isServerConnected;
    pthread_mutex_unlock(&mutex);
    return local;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    pthread_mutex_lock(&mutex);
    _isServerConnected = isServerConnected;
    pthread_mutex_unlock(&mutex);
}

const std::string &CashElementImpl::getHead() {
    return _head;
}

long long int CashElementImpl::getLength() {
    pthread_mutex_lock(&mutex);
    size_t local = _cash->size();
    pthread_mutex_unlock(&mutex);
    return local;
}

void CashElementImpl::memCopyFromCash(std::string *binaryString, long long int _countByteReadFromCash,
                                      long long int sizeCopy) {
    pthread_mutex_lock(&mutex);
    std::memcpy((void *) (binaryString)->c_str(), _cash->c_str() +
                                                  _countByteReadFromCash, sizeCopy);
    pthread_mutex_unlock(&mutex);
}

void CashElementImpl::appendStringToCash(std::string *binaryString) {
    pthread_mutex_lock(&mutex);
    _cash->append(*binaryString);
    pthread_mutex_unlock(&mutex);
}

void CashElementImpl::appendStringToCash(std::string binaryString) {
    pthread_mutex_lock(&mutex);
    _cash->append(binaryString);
    pthread_mutex_unlock(&mutex);
}
