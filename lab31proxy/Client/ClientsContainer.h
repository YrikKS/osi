//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CLIENTSCONTAINER_H
#define LAB31PROXY_CLIENTSCONTAINER_H

#include <list>
#include "Client.h"

namespace ProxyServer {
    class ClientsContainer {
    public:
        virtual std::list<Client>::iterator getIterator() = 0;
        virtual void addClient(Client client) = 0;
        virtual void delClient(std::list<Client>::iterator) = 0;
    };
}


#endif //LAB31PROXY_CLIENTSCONTAINER_H
