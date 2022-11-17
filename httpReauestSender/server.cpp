#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <sys/poll.h>
#include <termios.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define COUNT_BUFFER 330
#define HTTP_PORT 80
#define ERROR_CODE -1
#define EXIT_WITH_ERROR 1

int main(int argc, char *argv[]) {
    int sockFd = 0;
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        return 1;
    }

    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(atoi(argv[1]));
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_family = AF_INET;

    if (bind(sockFd, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) < 0) {
        perror("bind socket");
        return 1;
    }

    if (listen(sockFd, 3) < 0) {
        perror("listen socket");
        return 1;
    }

    int clientSock = 0;
    struct sockaddr clientAddr;
    socklen_t len = 0;
    if ((clientSock = accept(sockFd, (struct sockaddr *) &clientAddr, &len)) < 0) {
        perror("accept error");
    }
    std::cout << "connect" << std::endl;
//    write(sock, buffer, strlen(buffer)); // write(fd, char[]*, len);
//    bzero(buffer, BUFFER_SIZE);
    char buf[1024] = {0};
//    int pos = 0;
    while (read(clientSock, buf, BUFFER_SIZE - 1) != 0) {
        fprintf(stderr, "%s", buf);
        std::string buffer(buf);
        if (buffer.find("0\r\n") != std::string::npos) {
            bzero(buf, BUFFER_SIZE);
            break;
        }
        bzero(buf, BUFFER_SIZE);
    }

//    bool start = true;
    std::string str;
    while (true) {
        std::getline(std::cin, str);
        if (str == "end") {
//            str += "\r\n";
            std::string end = "0\r\n\r\n";
            write(clientSock, end.data(), end.size());
            break;
        } else {
            str += "\r\n";
            write(clientSock, str.data(), str.size());
        }
    }
    close(clientSock);
    close(sockFd);
    return 0;
}
