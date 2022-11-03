//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CLIENTSCONTAINERIMPL_H
#define LAB31PROXY_CLIENTSCONTAINERIMPL_H

#include "ClientsContainer.h"

namespace ProxyServer {
    class ClientsContainerImpl : public ClientsContainer{
    public:
        std::list<Client>::iterator getIterator() override;

        void addClient(Client client) override;

        void delClient(std::list<Client>::iterator iterator) override;

    private:
        std::list<Client> clientList;
    };
}


#endif //LAB31PROXY_CLIENTSCONTAINERIMPL_H
