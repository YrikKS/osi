//
// Created by kurya on 03.11.2022.
//

#include "ClientsContainerImpl.h"

std::list<ProxyServer::Client>::iterator ProxyServer::ClientsContainerImpl::getIterator() {
    return clientList.begin();
}

void ProxyServer::ClientsContainerImpl::addClient(Client client) {
//    clientList.push_back(client);
}

void ProxyServer::ClientsContainerImpl::delClient(std::list<ProxyServer::Client>::iterator iterator) {
    clientList.erase(iterator);
}
