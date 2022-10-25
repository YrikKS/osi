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
    if (connect(sock, (struct sockaddr *) &sockAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(1);
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
//    write(sock, buffer, strlen(buffer));
//    bzero(buffer, BUFFER_SIZE);

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    fd_set fd_in;
    fd_set fdConsole;
//    FD_ZERO(&fd_in);
//    FD_ZERO(&fdConsole);
//
//    FD_SET(sock, &fd_in);
//    FD_SET(fileno(stdin), &fdConsole);

    write(sock, buffer, strlen(buffer));
    bzero(buffer, BUFFER_SIZE);
    int check = 0;
    int check2 = 0;
    struct pollfd poll_set[2] = {0};
    poll_set[0].fd = sock;
    poll_set[0].events = POLLIN;
    poll_set[1].fd = fileno(stdin);
    poll_set[1].events = POLLIN;
//    bzero(bufferConsole, BUFFER_SIZE);
//    std::cout << "sock == " << sock << std::endl;
    struct termios old, newTerm;
    tcgetattr(STDIN_FILENO, &old);
    newTerm = old;
    newTerm.c_lflag &= ~(ICANON | ECHO);
    newTerm.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    while (true) {
        int ret = poll(poll_set, 2, 10000);
//        std::cout << ret << std::endl;
        if (ret == -1) {
            //error
            perror("select error");
            exit(1);
        } else if (ret == 0) {
            std::cout << "time out" << std::endl;
            break;
        } else {
            if (poll_set[0].revents & POLLIN ) {
                poll_set[0].revents = 0;
                std::cout << "read  ";
                std::cout.flush();
                read(sock, buffer, 80 * 25 - 1);
                fprintf(stdout, "%s", buffer);
                check2++;
                bzero(buffer, BUFFER_SIZE);
            }
            if (poll_set[1].revents & POLLIN) {
                poll_set[1].revents = 0;
                std::cout << "pleas write" << std::endl;
                std::cout.flush();
                char c;
                if (read(0, &c, 1) == -1) {
//                if (c == ' '){
                    perror("Read");
                }

                if (c == '\n') {
                    std::cout << "write ";
                }
                if (c == 'q') {
                    std::cout << "end ";
                    break;
                }
                check++;
            }
        }
    }
    std::cout << std::endl << check << std::endl;
    std::cout << std::endl << check2 << std::endl;


    close(sock);
    return 0;
}
