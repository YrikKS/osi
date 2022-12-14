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

//char **bufferInit() {
//    char **arrBuffer = (char **) malloc(sizeof(char *) * COUNT_BUFFER);
//    for (int i = 0; i < COUNT_BUFFER; i++) {
//        arrBuffer[i] = (char *) malloc(sizeof(char) * BUFFER_SIZE);
//    }
//    return arrBuffer;
//}
//
//void delBuffer(char **arrBuffer) {
//    for (int i = 0; i < COUNT_BUFFER; i++) {
//        free(arrBuffer[i]);
//    }
//    free(arrBuffer);
//}

std::string parseUrl(char *url) {
//    int sizeUrl = strlen(url);
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

int getPortFromUrl(std::string *url) {
    int index = url->find(":") + 1;
    int indexSlash = url->find("/");
    int port;
    if (indexSlash == std::string::npos) {
        port = atoi(url->substr(index).c_str());
        (*url) = url->substr(0, index - 1);
    } else {
        port = atoi(url->substr(index, indexSlash - index).c_str());
        (*url) = url->substr(0, index - 1) + url->substr(indexSlash);
    }
//    std::cout << url->substr(index, indexSlash - index) << std::endl;
    return port;
}

int connectSocket(std::string url, int port) {
    struct hostent *hostent = gethostbyname(url.data());
    if (hostent == NULL) {
        herror("gethostbyname");
        exit(EXIT_WITH_ERROR);
    }

    struct sockaddr_in sockAddr;
    bcopy(hostent->h_addr, &sockAddr.sin_addr, hostent->h_length);
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

void addToBuffer(std::vector<std::string> *vectorReadStrings, char *readBuf, std::string *restOfTheLine) {
    std::string readStrings(readBuf);
    int indexEnter = readStrings.find('\n', 0);
    vectorReadStrings->push_back((*restOfTheLine) + readStrings.substr(0, indexEnter + 1));
//    std::cout << (*vectorReadStrings)[vectorReadStrings->size()-1] << std::endl;
    int lastIndex = readStrings.find('\n', indexEnter + 1);

    while (lastIndex != readStrings.npos) {
        vectorReadStrings->push_back(readStrings.substr(indexEnter + 1, lastIndex - indexEnter));
        indexEnter = lastIndex;
        lastIndex = readStrings.find('\n', indexEnter + 1);
    }
    *restOfTheLine = readStrings.substr(indexEnter + 1);
//    std::cout << "restOfTheLine= " << *restOfTheLine << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect args!\n exampl: " << argv[0] << "<https://<hostName>" << std::endl;
        return 1;
    }
    std::string url = parseUrl(argv[1]); // ?????????????? http
    int port = getPortFromUrl(&url);
//    std::cout << url << "\nport = " << port << std::endl;
    std::string domain = getDomain(url);
    std::string path = getPath(url);
//    int port = atoi(argv[2]);
    int sock = connectSocket(domain, port);
    char buffer[BUFFER_SIZE] = {0};
    sprintf(buffer, "GET %s HTTP/1.1\r\nAccept: */*\r\nHost: %s:\r\nConnection: close\r\n\r\n", path.data(),
            domain.data());
    write(sock, buffer, strlen(buffer));
    bzero(buffer, BUFFER_SIZE);

    struct pollfd poll_set[2] = {0};
    poll_set[0].fd = sock;
    poll_set[0].events = POLLIN;
    poll_set[1].fd = 0;
    poll_set[1].events = POLLIN;
//    int file = open("file.txt", O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//    poll_set.fd = file;
//    poll_set.events = POLLIN;
//    char **bufferFromRead = bufferInit();
    std::vector<std::string> vectorReadStrings;
    std::string rest;
    int currentReadBuf = 0;
    int isPrint = 1;
    bool socketIsOpen = true;
//    std::cout << std::endl << "Press enter to scroll down" << std::endl;
    while (true) {
        int ret = poll(poll_set, 2, 100000); // 10000 == 100 src
        if (ret == -1) {
            perror("select error");
            exit(1);
        } else if (ret == 0) {
            std::cout << "time out" << std::endl;
//            break;
        } else {
            if (poll_set[0].revents & POLLIN) {
//                std::cout << poll_set[0].revents << " || " << (poll_set[0].revents & POLLIN) << std::endl;
                poll_set[0].revents = 0;
                int readByte;
                if (socketIsOpen) {
                    readByte = read(sock, buffer, BUFFER_SIZE - 1);
//                    std::cout << readByte << "   --   ";
//                    std::cout  << (int)buffer[readByte-5] << (int)buffer[readByte-4] << (int)buffer[readByte-3] << (int)buffer[readByte-2] << (int)buffer[readByte-1] << std::endl;
//                    if(buffer[readByte-5] == '0' && buffer[readByte-5] == '\n' && buffer[readByte-5] == '\r' &&
//                            buffer[readByte-5] == '\n' && buffer[readByte-5] == '\r') {
//                        std::cout << std::endl << "end write" << std::endl;
//                    }
                    addToBuffer(&vectorReadStrings, buffer, &rest);
//                    std::cout << isPrint << "and << " << (currentReadBuf < vectorReadStrings.size()) << std::endl;
                    while (isPrint &&
                           currentReadBuf < vectorReadStrings.size()) { //|| currentReadBuf < vectorReadStrings.size()
                        std::cout << vectorReadStrings[currentReadBuf];
                        std::cout.flush();
                        currentReadBuf++;
                        if (currentReadBuf % 25 == 0) {
                            std::cout << std::endl << "Press enter to scroll down" << std::endl;
                            isPrint = 0;
                        }
                    }
                    bzero(buffer, BUFFER_SIZE);
                }
                if (readByte == 0) {
                    socketIsOpen = false;
                }
                if (currentReadBuf >= vectorReadStrings.size() && !socketIsOpen) {
                    break;
                }
            }
            if (poll_set[1].revents & POLLIN) {
                poll_set[1].revents = 0;
                char c;
                if (read(0, &c, 1) == -1) {
                    perror("Read");
                    break;
                } else {
                    if (c == '\n') {
                        isPrint = 1;
                        while (isPrint && currentReadBuf <
                                          vectorReadStrings.size()) { //|| currentReadBuf < vectorReadStrings.size()
                            std::cout << vectorReadStrings[currentReadBuf];
                            std::cout.flush();
                            currentReadBuf++;
                            if (currentReadBuf % 25 == 0) {
                                std::cout << std::endl << "Press enter to scroll down" << std::endl;
                                isPrint = 0;
                            }
                        }
                        if (currentReadBuf >= vectorReadStrings.size() && !socketIsOpen) {
                            break;
                        }
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
    close(sock);
    return 0;
}

// ???????????????? ????????
// ???????????? 25 + ???????????????????? ????
// nc


// port :
//???????????????????? ?????? ??????????????????
// ?????????? ?????????????????? ?????????????? ???