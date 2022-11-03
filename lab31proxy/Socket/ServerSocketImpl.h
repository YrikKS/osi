//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_SERVERSOCKETIMPL_H
#define LAB31PROXY_SERVERSOCKETIMPL_H

#include "ServerSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include "../Constants.h"

namespace ProxyServer {
    class ServerSocketImpl : public ServerSocket{
    public:
        int connectSocket() override;

        int getFdSocket() override;

        int acceptNewClient() override;

    private:
        int fdSocket;
    };
}


#endif //LAB31PROXY_SERVERSOCKETIMPL_H
