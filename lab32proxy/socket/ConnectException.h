//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_CONNECTEXCEPTION_H
#define LAB31PROXY_CONNECTEXCEPTION_H

#include <exception>

namespace ProxyServer {
    class ConnectException : public std::exception {
    public:
        explicit ConnectException(std::string msg) : msg(std::move(msg)) {}

        const char* what() const _GLIBCXX_USE_NOEXCEPT override {
            return msg.c_str();
        }

    private:
        std::string msg{};
    };
}


#endif //LAB31PROXY_CONNECTEXCEPTION_H
