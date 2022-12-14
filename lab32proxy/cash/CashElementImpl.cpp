//
// Created by kurya on 07.11.2022.
//


#include "CashElementImpl.h"

using namespace ProxyServer;

bool CashElementImpl::isDownloadEnd() {
    return _isDownloadEnd;
}

void CashElementImpl::setDownloadEnd(bool var) {
    _isDownloadEnd = var;
}

std::shared_ptr<std::string> CashElementImpl::getCash() {
    return _cash;
}

CashElementImpl::CashElementImpl(std::string heading, long long int dataSize) {
    pthread_rwlock_init(&_mutexForData, NULL);
    pthread_mutex_init(&_mutexForSubscribers, NULL);
    _requestHeading = heading;
    std::hash<std::string> hasher;
    _cash->resize(dataSize - heading.size());
    _cash->clear();
    _hashRequestHeading = hasher(heading);
}

long long int CashElementImpl::getHash() {
    return _hashRequestHeading;
}

CashElementImpl::~CashElementImpl() {
    LOG_EVENT("delete cash ");
    errno = pthread_rwlock_destroy(&_mutexForData);
    if (errno != 0) {
        perror("destroy mutex error");
    }
    errno = pthread_mutex_destroy(&_mutexForSubscribers);
    if (errno != 0) {
        perror("destroy mutex error");
    }
}

int CashElementImpl::getCountUsers() {
    return _countConnectedUsers;
}

void CashElementImpl::addCountUsers() {
//    pthread_mutex_lock(&_mutexForData);
    _countConnectedUsers++;
//    pthread_mutex_unlock(&_mutexForData);
}

void CashElementImpl::minusCountUsers() {
//    pthread_mutex_lock(&_mutexForData);
    _countConnectedUsers--;
//    pthread_mutex_unlock(&_mutexForData);
}

bool CashElementImpl::isIsServerConnected() {
    return _isServerConnected;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    _isServerConnected = isServerConnected;

    if (!isServerConnected) {
        signalUsers();
    }

}

const std::string &CashElementImpl::getHeading() {
    return _requestHeading;
}

long long int CashElementImpl::getLength() {
    return _cash->size();
}

void CashElementImpl::memCopyFromCash(std::string *target, long long int offset,
                                      long long int sizeCopy) {
    pthread_rwlock_rdlock(&_mutexForData);
    memcpy((void *) (target)->c_str(), _cash->c_str() +
                                       offset, sizeCopy);
//    std::memcpy((void *) (target)->c_str(), _cash->c_str() +
//                                                  offset, sizeCopy);
    pthread_rwlock_unlock(&_mutexForData);
}

void CashElementImpl::appendStringToCash(std::string *binaryString) {
    pthread_rwlock_wrlock(&_mutexForData);
    _cash->append(*binaryString);
    pthread_rwlock_unlock(&_mutexForData);
    signalUsers();
}

void CashElementImpl::addCondVar(pthread_cond_t *condVar) {
    pthread_mutex_lock(&_mutexForSubscribers);
    _listSubscribers.push_front(condVar);
    pthread_mutex_unlock(&_mutexForSubscribers);
}

void CashElementImpl::dellCondVar(pthread_cond_t *condVar) {
    pthread_mutex_lock(&_mutexForSubscribers);
    for (auto it = _listSubscribers.begin(); it != _listSubscribers.end(); it++) {
        if ((*it) == condVar) {
            _listSubscribers.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&_mutexForSubscribers);
}

void CashElementImpl::signalUsers() {
    pthread_mutex_lock(&_mutexForSubscribers);
    for (auto &item: _listSubscribers) {
        pthread_cond_signal(item);
    }
    pthread_mutex_unlock(&_mutexForSubscribers);
}

// list subers
//blokirovka на течение \ запись mutex lock
// удалить на атомарных операциях
// передавать указщатель в append
// (нэйминг поправить)
