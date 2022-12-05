//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENTIMPL_H
#define LAB31PROXY_CASHELEMENTIMPL_H

#include <memory>
#include <list>
#include "CashElement.h"


namespace ProxyServer {
    class CashElementImpl : public CashElement {
    public:
        explicit CashElementImpl(std::string heading);

        bool isCashEnd() override;

        void setIsCashEnd(bool var) override;

        std::shared_ptr<std::string> getCash() override;

        long long int getHash() override;

        ~CashElementImpl() override;

        void addUser(Client *client) override;

        void dellUser(Client *client) override;

        std::list<Client *> getGetUsers() override;

        bool isIsServerConnected() override;

        void setIsServerConnect(bool isServerConnected) override;

        const std::string &getHead() override;

    private:
        std::list<Client*> _connectionsUsers;
        bool _isCashEnd = false;
        bool _isServerConnected = false;
        long long int _hashHead = 0;
        std::string _head;
        std::shared_ptr<std::string> _cash = std::make_shared<std::string>();
    };
}

#endif //LAB31PROXY_CASHELEMENTIMPL_H
