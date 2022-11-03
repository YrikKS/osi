//
// Created by kurya on 03.11.2022.
//

#include <iostream>
#include "ServerSocketImpl.h"

int ProxyServer::ServerSocketImpl::connectSocket() {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return 1;
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(PORT_SERVER_SOCKET);
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_family = AF_INET;

    if (bind(sockFd, (struct sockaddr*) &sockAddr, sizeof(struct sockaddr_in)) < 0) {                                                                                         // назначаем имя сокету
        perror("bind");
        return 2;
    }
    std::cout << "listen" << std::endl;
    if (listen(sockFd, 3) < 0) {                                                                                                                                                 // выразить готовность принимать входящие соединения и задать размер очереди
        perror("listen");
        return 3;
    }

    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;                                                                                                                                                             //  перед вызовом он содержит размер структуры, на которую ссылается addr, а после вызова~-- действительную длину адреса в байтах
    std::cout << "accept" << std::endl;
    if ((clientSock = accept(sockFd, (struct sockaddr *)&clientAddr, &len)) < 0) {                                                                                                         // вытаскивает из очереди 1 элемент и устанавливает соединение
        perror("accept");
        return 4;
    }
    std::cout << "read" << std::endl;
    char buffer[1024] = {0};
    int strSize = 0;
    while ((strSize = recv(clientSock, buffer, 1023, 0) > 0)) {
        for (int i = 0; i < strSize; ++i){
            printf("%c", toupper(buffer[i]));
        }
        printf("\n");
    }
}

int ProxyServer::ServerSocketImpl::getFdSocket() {
    return fdSocket;
}

int ProxyServer::ServerSocketImpl::acceptNewClient() {

    return 0;
}
