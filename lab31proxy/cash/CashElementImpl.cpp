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

bool CashElementImpl::isIsServerConnected() {
    return _isServerConnected;
}

void CashElementImpl::setIsServerConnect(bool isServerConnected) {
    _isServerConnected = isServerConnected;
}

const std::string &CashElementImpl::getHead() {
    return _head;
}

void CashElementImpl::addUser(Client *client) {
    for (auto it = _connectionsUsers.begin(); it != _connectionsUsers.end(); it++) {
        if(*it == client) {
            return;
        }
    }
    _connectionsUsers.push_back(client);
}

void CashElementImpl::dellUser(Client *client) {
    for (auto it = _connectionsUsers.begin(); it != _connectionsUsers.end(); it++) {
        if(*it == client) {
            it = _connectionsUsers.erase(it);
        }
    }
}

std::list<Client *> CashElementImpl::getGetUsers() {
    return _connectionsUsers;
}
