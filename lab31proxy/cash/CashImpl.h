//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHIMPL_H
#define LAB31PROXY_CASHIMPL_H

#include <list>
#include "Cash.h"
#include "../Constants.h"

namespace ProxyServer {
    class CashImpl : public Cash{
    public:
        ~CashImpl() override;

        CashElement *findResponseInCash(std::string heading) override;

        CashElement *addStringToCash(std::string request, long long int dataSize) override;

        bool isElementInCash(std::string request) override;

    private:
        std::list<CashElement*> _listCash;
        long long int byteInCash;
    };
}


#endif //LAB31PROXY_CASHIMPL_H
