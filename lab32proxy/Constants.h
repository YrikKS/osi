//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CONSTANTS_H
#define LAB31PROXY_CONSTANTS_H
#define ERROR_CODE -1
namespace ProxyServer {
    static const int TIME_OUT_POLL = 10000;
    static const int PORT_SERVER_SOCKET = 7777; //???
    static const int MAX_COUNT_CONNECTIONS = 65000;
//    static const long long int BUF_SIZE = 1024 * 1024;
    #define BUF_SIZE 1024 * 512
//    static const long long int BUF_SIZE = 1024;
    static int DEFAULT_PORT = 80;
    static int COUNT_CASH_ELEMENT = 100;
    static long long int SIZE_EACH_CASH_ELEMENT = 9223372036854775000;
//    static long long int SIZE_EACH_CASH_ELEMENT = 1024 * 1024;
    static long long int MAX_CASH_SIZE = 1024 * 1024 * 1024; // 1 гб?

    static std::string REGEX_FOR_HOST_AND_PORT = "Host: ([\\w.\\d-]*)(:(\\d)+)*";
    static std::string REGEX_FOR_CONTENT_LENGTH = "Content-Length: ([\\d]*)";
}

#endif //LAB31PROXY_CONSTANTS_H
