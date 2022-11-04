//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_PARSERIMPL_H
#define LAB31PROXY_PARSERIMPL_H

#include "ParserTypeRequest.h"

namespace ProxyServer {
    class ParserImpl {
    public:
        static TypeRequest parsingRequest(char *buf);
    };
}


#endif //LAB31PROXY_PARSERIMPL_H
