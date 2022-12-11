//
// Created by kurya on 10.12.2022.
//

#ifndef LAB32PROXY_HANDLERONECLIENT_H
#define LAB32PROXY_HANDLERONECLIENT_H

#include "../client/Client.h"
#include <poll.h>

namespace ProxyServer {
    class HandlerOneClient {
    public:
        virtual ~HandlerOneClient() = default;

        virtual void startHandler() = 0;
    };

}

#endif //LAB32PROXY_HANDLERONECLIENT_H
