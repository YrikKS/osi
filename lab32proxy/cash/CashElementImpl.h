//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENTIMPL_H
#define LAB31PROXY_CASHELEMENTIMPL_H

#include <memory>
#include <cstring>
#include <list>
#include "CashElement.h"


namespace ProxyServer {
    class CashElementImpl : public CashElement {
    public:
        explicit CashElementImpl(std::string heading, long long int dataSize);

        bool isDownloadEnd() override;

        void setDownloadEnd(bool var) override;

        std::shared_ptr<std::string> getCash() override;

        long long int getHash() override;

        ~CashElementImpl() override;

        int getCountUsers() override;

        void addCountUsers() override;

        void minusCountUsers() override;

        bool isIsServerConnected() override;

        void setIsServerConnect(bool isServerConnected) override;

        const std::string &getHeading() override;

        long long int getLength() override;

        void memCopyFromCash(std::string *target, long long int offset,
                             long long int sizeCopy) override;

        void appendStringToCash(std::string *binaryString) override;

        void addCondVar(pthread_cond_t *condVar) override;

        void dellCondVar(pthread_cond_t *condVar) override;
    private:
        int _countConnectedUsers = 0;
        bool _isDownloadEnd = false;
        bool _isServerConnected = false;
        long long int _hashRequestHeading = 0;
        std::string _requestHeading;
        std::shared_ptr<std::string> _cash = std::make_shared<std::string>();
        std::list<pthread_cond_t*> _listSubscribers;
        void signalUsers();
        pthread_mutex_t _mutexForSubscribers ;
        pthread_rwlock_t _mutexForData ; // блокировки
    };
}

#endif //LAB31PROXY_CASHELEMENTIMPL_H
