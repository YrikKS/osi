#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#include <netinet/tcp.h>
//#include <sys/socket.h>
#include <sys/types.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <iostream>


std::string parseUrl(char* url) {
    int sizeUrl = strlen(url);
    std::string strUrl = url;
    std::string subSting;
    if(strUrl.find("http://") != strUrl.npos) {
        subSting = strUrl.substr(7);
    } else if(strUrl.find("https://") != strUrl.npos) {
        subSting = strUrl.substr(8);
    } else {
        subSting = strUrl;
    }
    return subSting;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cout << "Incorrect args!\n exampl: " << argv[0] << "<https://<hostName>" << std::endl;
        return 1;
    }

    std::string url = parseUrl(argv[1]);
    std::cout << url << std::endl;
}