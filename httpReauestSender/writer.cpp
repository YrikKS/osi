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

    struct hostent *hostent = gethostbyname("localhost");
    if (hostent == NULL) {
        herror("gethostbyname");
        exit(EXIT_WITH_ERROR);
    }

    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
    int port = atoi(argv[1]);
    sockAddr.sin_port = htons(port);
    sockAddr.sin_family = AF_INET;

    int sock = socket(AF_INET, SOCK_STREAM, 0); //PF_INET
    if (sock == ERROR_CODE) {
        perror("setsockopt");
        exit(EXIT_WITH_ERROR);
    }
    if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == ERROR_CODE) {
        perror("connect");
        exit(EXIT_WITH_ERROR);
    }


    std::string str;
    while (true) {
        std::getline(std::cin, str);
        if (str == "end") {
            break;
        } else {
            str += "\r\n";
            write(sock, str.data(), str.size());
        }
    }

    char buffer[BUFFER_SIZE] = {0};
    while (read(sock, buffer, BUFFER_SIZE - 1) != 0) {
        fprintf(stderr, "%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }
    return 0;
}
