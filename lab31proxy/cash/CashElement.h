//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENT_H
#define LAB31PROXY_CASHELEMENT_H

#include <string>
#include "../binaryString/BinaryString.h"
#include "../logger/Logger.h"
//#include "../observer/Observable.h"
#include <memory>

namespace ProxyServer {
    class CashElement {
    public:
        virtual ~CashElement() = default;

//        virtual void addToObserver(Client* client) = 0;

        virtual bool isDownloadEnd() = 0;

        virtual void setDownloadEnd(bool var) = 0;

        virtual std::shared_ptr<std::string> getCash() = 0;

        virtual long long int getHash() = 0;

        virtual int getCountUsers() = 0;

        virtual void addCountUsers() = 0;

        virtual void minusCountUsers() = 0;

        virtual bool isIsServerConnected() = 0;

        virtual void setIsServerConnect(bool isServerConnected) = 0;

        virtual std::string &getHeading() = 0;
    };
}
#endif //LAB31PROXY_CASHELEMENT_H
