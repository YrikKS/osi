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
#include <sys/poll.h>
#include <termios.h>

#define BUFFER_SIZE 1024
#define COUNT_BUFFER 330
#define HTTP_PORT 80
#define ERROR_CODE -1
#define EXIT_WITH_ERROR 1

char **bufferInit() {
    char **arrBuffer = (char **) malloc(sizeof(char *) * COUNT_BUFFER);
    for (int i = 0; i < COUNT_BUFFER; i++) {
        arrBuffer[i] = (char *) malloc(sizeof(char) * BUFFER_SIZE);
    }
    return arrBuffer;
}

void delBuffer(char **arrBuffer) {
    for (int i = 0; i < COUNT_BUFFER; i++) {
        free(arrBuffer[i]);
    }
    free(arrBuffer);
}

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
    if (hostent == NULL) {
        herror("gethostbyname");
        exit(EXIT_WITH_ERROR);
    }

    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
    sockAddr.sin_port = htons(HTTP_PORT);
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
    return sock;
}

std::string getPath(std::string url) {
    int indexEndHostName = url.find("/");
    if (indexEndHostName == url.npos) {
        return "/";
    } else {
        return url.substr(indexEndHostName);
    }
}

std::string getDomain(std::string url) {
    int indexEndHostName = url.find("/");
    if (indexEndHostName == url.npos) {
        return url;
    } else {
        return url.substr(0, indexEndHostName);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect args!\n exampl: " << argv[0] << "<https://<hostName>" << std::endl;
        return 1;
    }

    std::string url = parseUrl(argv[1]); // убираем http
    std::string domain = getDomain(url);
    std::string path = getPath(url);
    int sock = connectSocket(domain);
    char buffer[BUFFER_SIZE] = {0};
    sprintf(buffer, "GET %s HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\n\r\n", path.data(), domain.data());

    write(sock, buffer, strlen(buffer));

    struct pollfd poll_set[2] = {0};
    poll_set[0].fd = sock;
    poll_set[0].events = POLLIN;
    poll_set[1].fd = 0;
    poll_set[1].events = POLLIN;

    char **bufferFromRead = bufferInit();
    int currentReadBuf = 0;
    int currentWriteBuf = 0;
    bool socketIsOpen = true;
    while (true) {
        int ret = poll(poll_set, 2, 10000);
        if (ret == -1) {
            perror("select error");
            exit(1);
        } else if (ret == 0) {
            std::cout << "time out" << std::endl;
            break;
        } else {
            if (poll_set[0].revents & POLLIN) {
                poll_set[0].revents = 0;
                int readByte;
                if (socketIsOpen) {
                    readByte = read(sock, bufferFromRead[currentReadBuf], BUFFER_SIZE - 1);
                }
                if (readByte == 0) {
                    socketIsOpen = false;
                } else {
                    currentReadBuf++;
                    std::cout << "currentReadBuf == " << currentReadBuf << std::endl;
                    std::cout.flush();
                }
            }
            if (poll_set[1].revents & POLLIN) {
                poll_set[1].revents = 0;
                char c;
                if (read(0, &c, 1) == -1) {
                    perror("Read");
                    break;
                } else {
                    if (c == '\n' && currentWriteBuf < currentReadBuf + 1) {
                        fprintf(stdout, "%s", bufferFromRead[currentWriteBuf]);
                        currentWriteBuf++;
                        std::cout << currentWriteBuf << std::endl;
                        std::cout << std::endl << "Press enter to scroll down" << std::endl;
                    } else if (c == '\n') {
                        std::cout << "pleas wait data" << std::endl;
                    }
                    if (c == 'q') {
                        std::cout << "end ";
                        break;
                    }
                }
            }
        }
    }
    std::cout << "all good" << std::endl;
    delBuffer(bufferFromRead);
    close(sock);
    return 0;
}
