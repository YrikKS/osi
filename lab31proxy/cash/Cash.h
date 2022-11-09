//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASH_H
#define LAB31PROXY_CASH_H

#include <string>
#include "CashElement.h"

namespace ProxyServer {
    class Cash {
    public:
        virtual ~Cash() = default;

        virtual CashElement *findResponseInCash(std::string heading) = 0;

        virtual CashElement *addStringToCash(std::string request) = 0;
    };
}
#endif //LAB31PROXY_CASH_H