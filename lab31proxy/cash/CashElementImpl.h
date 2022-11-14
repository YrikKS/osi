//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENTIMPL_H
#define LAB31PROXY_CASHELEMENTIMPL_H

#include "CashElement.h"


namespace ProxyServer {
    class CashElementImpl : public CashElement {
    public:
        explicit CashElementImpl(std::string heading);

        bool isCashEnd() override;

        void setIsCashEnd() override;

        BinaryString *getCash() override;

        size_t getHash() override;

        ~CashElementImpl() override;
    private:
        bool _isCashEnd = false;
        size_t _hashHead = 0;
        std::string _head;
        BinaryString _cash;
    };
}

#endif //LAB31PROXY_CASHELEMENTIMPL_H
