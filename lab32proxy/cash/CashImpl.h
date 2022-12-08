//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHIMPL_H
#define LAB31PROXY_CASHIMPL_H

#include <list>
#include <pthread.h>
#include "Cash.h"
#include "../Constants.h"

namespace ProxyServer {
    class CashImpl : public Cash {
    public:
        CashImpl();

        ~CashImpl() override;

        CashElement *findResponseInCash(std::string heading) override;

        CashElement *addStringToCash(std::string request, long long int dataSize) override;

        bool isElementInCash(std::string request) override;

    private:
        pthread_mutex_t mutex;
        std::list<CashElement *> _listCash;
        long long int byteInCash;
    };
}


#endif //LAB31PROXY_CASHIMPL_H
