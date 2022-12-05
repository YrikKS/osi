//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENT_H
#define LAB31PROXY_CASHELEMENT_H

#include <string>
#include "../binaryString/BinaryString.h"
#include "../logger/Logger.h"
#include "../client/Client.h"
#include <memory>
#include <list>

namespace ProxyServer {
    class CashElement {
    public:
        virtual ~CashElement() = default;

        virtual bool isCashEnd() = 0;

        virtual void setIsCashEnd(bool var) = 0;

        virtual std::shared_ptr<std::string> getCash() = 0;

        virtual long long int getHash() = 0;

        virtual std::list<Client*> getGetUsers() = 0;

        virtual void addUser(Client* client) = 0;

        virtual void dellUser(Client* client) = 0;

        virtual bool isIsServerConnected() = 0;

        virtual void setIsServerConnect(bool isServerConnected) = 0;

        virtual const std::string &getHead() = 0;
    };
}
#endif //LAB31PROXY_CASHELEMENT_H
