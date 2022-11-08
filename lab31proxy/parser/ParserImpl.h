//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_PARSERIMPL_H
#define LAB31PROXY_PARSERIMPL_H

#include "ParserTypeRequest.h"
#include "ResultParseHeading.h"
#include "ParseException.h"
#include "../logger/Logger.h"
#include "../Constants.h"

namespace ProxyServer {
    class ParserImpl {
    public:
        static TypeRequestAndResponse parsingRequest(char *buf, char* host);
        static ResultPars findEndHeading(std::string buf, int* posEnd);
        static ResultPars findEndBody(std::string buffer, int* posEnd);
        static ResultParseHeading* parsingHeading(std::string heading);
        static ResultParseHeading parsingResponseHeading(std::string heading);

    private:
        static void findHostAndPort(ResultParseHeading* result, std::string buf);
        static void findContentLength(ResultParseHeading* result, std::string buf);
    };
}


#endif //LAB31PROXY_PARSERIMPL_H
