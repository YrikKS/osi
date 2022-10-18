#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/termios.h>
#include <iostream>

int main(int argc, char *argv[]) {
    char pszRequest[100] = {0};
    char pszResourcePath[] = "";
    char pszHostAddress[] = "https://www.opennet.ru/man.shtml?topic=socket&category=2&russian=0";
    sprintf(pszRequest, "GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", pszResourcePath,
            pszHostAddress);
    printf("Created Get Request is below:\n\n\n");
    struct hostent *hp;
    hp = gethostbyname(pszHostAddress);
    std::cout << hp->h_addr << std::endl;
//
//    char ADDRESS[] = "https://www.opennet.ru/man.shtml?topic=socket&category=2&russian=0";
//    int sockfd = 0;
//    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){                                                                                                                                // возращает дискриптор sockfd ()
//        perror("client socket");
//        return 1;
//    }
//
//    struct sockaddr_un sockAddress;                                                                                                                                              // Адрес доменного сокета UNIX
//    memset(&sockAddress, 0, sizeof(struct sockaddr_un));                                                                                                                                    //инициализируем переменную нулями
//
//    sockAddress.sun_family = PF_INET;                                                                                                                                                // заполняем поле sun_family
//    int addressLen = strlen(ADDRESS);                                                                                                                                               // узнаем размер ADDRESS
//    strncpy(sockAddress.sun_path, ADDRESS, addressLen);
//
//
//    std::cout << "connect" << std::endl;
//    if (connect(sockfd, (struct sockaddr*) &sockAddress, sizeof(sockAddress)) < 0){
//        perror("connect");
//        return 2;
//    }
//
//    std::cout << "all good" << std::endl;
//    close(sockfd);
//    send(sockfd, pszRequest, 100, 0);
//    char buffer[1024] = {0};

    return 0;
}