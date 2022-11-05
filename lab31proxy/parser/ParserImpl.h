//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_PARSERIMPL_H
#define LAB31PROXY_PARSERIMPL_H

#include "ParserTypeRequest.h"
#include "ResultParseHeading.h"
#include "ParseException.h"
#include "../logger/Logger.h"

namespace ProxyServer {
    class ParserImpl {
    public:
        static TypeRequest parsingRequest(char *buf, char* host);
        static ResultPars findEndHeading(char* buf, int* posEnd);
        static ResultPars findEndBody(char* buf, int* posEnd);
        static ResultParseHeading* parsingHeading(std::string heading);
    };
}


#endif //LAB31PROXY_PARSERIMPL_H
