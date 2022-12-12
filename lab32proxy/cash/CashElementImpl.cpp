//
// Created by kurya on 07.11.2022.
//


#include "CashElementImpl.h"

using namespace ProxyServer;

bool CashElementImpl::isCashEnd() {
    pthread_mutex_lock(&mutexForData);
    bool local = _isCashEnd;
    pthread_mutex_unlock(&mutexForData);
    return local;
}

void CashElementImpl::setIsCashEnd(bool var) {
    pthread_mutex_lock(&mutexForData);
    _isCashEnd = var;
    pthread_mutex_unlock(&mutexForData);
}

std::shared_ptr<std::string> CashElementImpl::getCash() {
    return _cash;
}

CashElementImpl::CashElementImpl(std::string heading) {
    pthread_mutex_init(&mutexForData, NULL);
    pthread_mutex_init(&mutexForList, NULL);
    pthread_mutex_init(&mutexForCopy, NULL);
    _head = heading;
    std::hash<std::string> hasher;
    _hashHead = hasher(heading);
}

long long int CashElementImpl::getHash() {
    return _hashHead;
}

CashElementImpl::~CashElementImpl() {
    LOG_EVENT("delete cash ");
    errno = pthread_mutex_destroy(&mutexForData);
    if (errno != 0) {
        perror("destroy mutex error");
    }
    errno = pthread_mutex_destroy(&mutexForList);
    if (errno != 0) {
        perror("destroy mutex error");
    }
}

int CashElementImpl::getCountUsers() {
    pthread_mutex_lock(&mutexForData);
    int local = _countUsers;
    pthread_mutex_unlock(&mutexForData);
    return local;
}

void CashElementImpl::addCountUsers() {
    pthread_mutex_lock(&mutexForData);
    _countUsers++;
    pthread_mutex_unlock(&mutexForData);
}

void CashElementImpl::minusCountUsers() {
    pthread_mutex_lock(&mutexForData);
    _countUsers--;
    pthread_mutex_unlock(&mutexForData);
}

bool CashElementImpl::isIsServerConnected() {
    pthread_mutex_lock(&mutexForData);
    size_t local = _isServerConnected;
    pthread_mutex_unlock(&mutexForData);
    return local;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    pthread_mutex_lock(&mutexForData);
    _isServerConnected = isServerConnected;
    pthread_mutex_unlock(&mutexForData);

    if (!isServerConnected) {
        signalUsers();
    }

}

const std::string &CashElementImpl::getHead() {
    return _head;
}

long long int CashElementImpl::getLength() {
    pthread_mutex_lock(&mutexForData);
    long long int local = _cash->size();
    pthread_mutex_unlock(&mutexForData);
    return local;
}

void CashElementImpl::memCopyFromCash(std::string *binaryString, long long int _countByteReadFromCash,
                                      long long int sizeCopy) {
    pthread_mutex_lock(&mutexForData);
    memcpy((void *) (binaryString)->c_str(), _cash->c_str() +
                                             _countByteReadFromCash, sizeCopy);
//    std::memcpy((void *) (binaryString)->c_str(), _cash->c_str() +
//                                                  _countByteReadFromCash, sizeCopy);
    pthread_mutex_unlock(&mutexForData);
}

void CashElementImpl::appendStringToCash(std::string *binaryString) {
    pthread_mutex_lock(&mutexForData);
    _cash->append(*binaryString);
    pthread_mutex_unlock(&mutexForData);
    signalUsers();
}

void CashElementImpl::appendStringToCash(std::string binaryString) {
    pthread_mutex_lock(&mutexForData);
    _cash->append(binaryString);
    pthread_mutex_unlock(&mutexForData);
    signalUsers();
}

void CashElementImpl::addCondVar(pthread_cond_t *condVar) {
    pthread_mutex_lock(&mutexForList);
    listUsers.push_front(condVar);
    pthread_mutex_unlock(&mutexForList);
}

void CashElementImpl::dellCondVar(pthread_cond_t *condVar) {
    pthread_mutex_lock(&mutexForList);
    for (auto it = listUsers.begin(); it != listUsers.end(); it++) {
        if ((*it) == condVar) {
            listUsers.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&mutexForList);
}

void CashElementImpl::signalUsers() {
    pthread_mutex_lock(&mutexForList);
    std::cout << "signal cash" << std::endl;
    for (auto &item: listUsers) {
        pthread_cond_signal(item);
    }
    std::cout << "end signal cash size listUsers == " << listUsers.size() << std::endl;
    pthread_mutex_unlock(&mutexForList);
}

pthread_mutex_t* CashElementImpl::getMutex() {
    return &mutexForCopy;
}
