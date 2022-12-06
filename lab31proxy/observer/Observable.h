//
// Created by kurya on 06.12.2022.
//

#ifndef LAB31PROXY_OBSERVABLE_H
#define LAB31PROXY_OBSERVABLE_H

#include <list>
#include "../client/Client.h"

namespace ProxyServer {
    class Observable {
    public:
        const std::list<Client *> &getListClient() const;

        void setListClient(const std::list<Client *> &listClient);

    private:
        std::list<Client*> listClient;
    };
}

#endif //LAB31PROXY_OBSERVABLE_H
