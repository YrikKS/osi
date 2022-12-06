//
// Created by kurya on 06.12.2022.
//

#include "Observable.h"

using namespace ProxyServer;
const std::list<Client *> &ProxyServer::Observable::getListClient() const {
    return listClient;
}

void ProxyServer::Observable::setListClient(const std::list<Client *> &listClient) {
    Observable::listClient = listClient;
}
