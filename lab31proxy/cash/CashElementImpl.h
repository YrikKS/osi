//
// Created by kurya on 07.11.2022.
//

#ifndef LAB31PROXY_CASHELEMENTIMPL_H
#define LAB31PROXY_CASHELEMENTIMPL_H

#include <memory>
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

        int getCountUsers() override;

        void addCountUsers() override;

        void minusCountUsers() override;

        bool isIsServerConnected() override;

        void setIsServerConnect(bool isServerConnected) override;

        const std::string &getHead() override;

    private:
        int _countUsers = 0;
        bool _isCashEnd = false;
        bool _isServerConnected = false;
        long long int _hashHead = 0;
        std::string _head;
        std::shared_ptr<std::string> _cash = std::make_shared<std::string>();
    };
}

#endif //LAB31PROXY_CASHELEMENTIMPL_H
