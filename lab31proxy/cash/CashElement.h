//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENT_H
#define LAB31PROXY_CASHELEMENT_H

#include <string>

namespace ProxyServer {
    class CashElement {
    public:
        virtual ~CashElement() = default;

        virtual bool isCashEnd() = 0;

        virtual void setIsCashEnd() = 0;

        virtual std::string *getCash() = 0;

        virtual size_t getHash() = 0;
    };
}
#endif //LAB31PROXY_CASHELEMENT_H