//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENT_H
#define LAB31PROXY_CASHELEMENT_H

#include <string>
#include "../logger/Logger.h"
//#include "../observer/Observable.h"
#include <memory>

namespace ProxyServer {
    class CashElement {
    public:
        virtual ~CashElement() = default;

        virtual bool isDownloadEnd() = 0;

        virtual void setDownloadEnd(bool var) = 0;

        virtual std::shared_ptr<std::string> getCash() = 0;

        virtual long long int getHash() = 0;

        virtual int getCountUsers() = 0;

        virtual void addCountUsers() = 0;

        virtual void minusCountUsers() = 0;

        virtual bool isIsServerConnected() = 0;

        virtual void setIsServerConnect(bool isServerConnected) = 0;

        virtual const std::string &getHeading() = 0;

        virtual long long int getLength() = 0;

        virtual void memCopyFromCash(std::string *binaryString,
                                     long long int _countByteReadFromCash, long long int sizeCopy) = 0;

        virtual void appendStringToCash(std::string *binaryString) = 0;

        virtual void addCondVar(pthread_cond_t *condVar) = 0;

        virtual void dellCondVar(pthread_cond_t *condVar) = 0;
    };
}
#endif //LAB31PROXY_CASHELEMENT_H
