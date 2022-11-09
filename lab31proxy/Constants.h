//
// Created by kurya on 03.11.2022.
//

#ifndef LAB31PROXY_CONSTANTS_H
#define LAB31PROXY_CONSTANTS_H
#define ERROR_CODE -1
namespace ProxyServer {
    static const int TIME_OUT_POLL = 10000;
    static const int PORT_SERVER_SOCKET = 8080; //???
    static const int MAX_COUNT_CONNECTIONS = 1024;
    static const int BUF_SIZE = 1024;
    static int DEFAULT_PORT = 80;
    static int COUNT_CASH_ELEMENT = 100;
    static int SIZE_EACH_CASH_ELEMENT = 1024 * 1024;
    static int MAX_CASH_SIZE = 1024 * 1024; // 1 кб

    static std::string REGEX_FOR_HOST_AND_PORT = "Host: ([\\w.\\d-]*)(:(\\d)+)*";
    static std::string REGEX_FOR_CONTENT_LENGTH = "Content-Length: ([\\d]*)";
}

#endif //LAB31PROXY_CONSTANTS_H
