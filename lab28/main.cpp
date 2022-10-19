#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#define BUFFER_SIZE 1024

std::string parseUrl(char *url) {
    int sizeUrl = strlen(url);
    std::string strUrl = url;
    std::string subSting;
    if (strUrl.find("http://") != strUrl.npos) {
        subSting = strUrl.substr(7);
    } else if (strUrl.find("https://") != strUrl.npos) {
        subSting = strUrl.substr(8);
    } else {
        subSting = strUrl;
    }
    return subSting;
}

int connectSocket(std::string url) {
    struct hostent *hostent = gethostbyname(url.data());
//    struct sockaddr_in addr;
//    int on = 1, sock;
//    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
//    addr.sin_port = htons(port);
//    addr.sin_family = AF_INET;
//    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
//
//    if(sock == -1){
//        perror("setsockopt");
//        exit(1);
//    }
//
//    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
//        perror("connect");
//        exit(1);
//
//    }
//    return sock;
    if (hostent == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
    sockAddr.sin_port = htons(80);
    sockAddr.sin_family = AF_INET;

    int sock = socket(AF_INET, SOCK_STREAM, 0); //PF_INET
    if (sock == -1) {
        perror("setsockopt");
        exit(1);
    }
//    int on = 1;
//    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(1);
    }
    return sock;
}

//void sendRequest

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect args!\n exampl: " << argv[0] << "<https://<hostName>" << std::endl;
        return 1;
    }
    std::string url = parseUrl(argv[1]);
    int sock = connectSocket(url);

    char buffer[BUFFER_SIZE] = "GET /WackoWiki/KursOperacionnyeSistemy/PraktikumPosixThreads/PthreadTasks HTTP/1.1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nHost: parallels.nsu.ru\r\n\r\n";
    write(sock, buffer, strlen(buffer)); // write(fd, char[]*, len);
    bzero(buffer, BUFFER_SIZE);

//    struct timeval tv;
//    tv.tv_sec = 5;
//    tv.tv_usec = 0;

    while (read(sock, buffer, BUFFER_SIZE - 1) != 0) {
//    while(select() != 0)
        fprintf(stderr, "%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }

    close(sock);
    return 0;
}

//netcat