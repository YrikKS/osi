
//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVER_H
#define LAB31PROXY_SERVER_H

namespace ProxyServer {
    class Server {
    public:
        virtual void startServer() = 0;
        virtual ~Server() = default;
    };
}

#endif //LAB31PROXY_SERVER_H
