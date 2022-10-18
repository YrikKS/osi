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

int socket_connect(char *host, in_port_t port){
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;

    if((hp = gethostbyname(host)) == NULL){
        herror("gethostbyname");
        exit(1);
    }
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if(sock == -1){
        perror("setsockopt");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
        perror("connect");
        exit(1);

    }
    return sock;
}

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int fd;
    char buffer[BUFFER_SIZE];

    if(argc < 3){
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }

    fd = socket_connect(argv[1], atoi(argv[2]));
    char pszRequest[100]= {0};
    char pszResourcePath[] = "Dross0/OS-Labs/blob/master/HTTP%20Client%201/http.c";
//    https://github.com/Dross0/OS-Labs/blob/master/HTTP%20Client%201/http.c
    char* pszHostAddress = argv[1];
    sprintf(pszRequest, "GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", pszResourcePath, pszHostAddress);
    std::cout << pszRequest << std::endl;

    write(fd, pszRequest, strlen(pszRequest)); // write(fd, char[]*, len);
    bzero(buffer, BUFFER_SIZE);

    while(read(fd, buffer, BUFFER_SIZE - 1) != 0){
        fprintf(stderr, "%s", buffer);
        bzero(buffer, BUFFER_SIZE);
    }

    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}