//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_PARSERTYPEREQUEST_H
#define LAB31PROXY_PARSERTYPEREQUEST_H
namespace ProxyServer {
    typedef enum {
        GET_REQUEST,
        NORMAL_RESPONSE,
        NOT_CASH_RESPONSE,
        GET_REQUEST_NOT_CASH,
        NOT_GET_REQUEST,
        INVALID
    } TypeRequestAndResponse;

    typedef enum {
        END_HEADING,
        END_BODY,
        NOTHING
    } ResultPars;
}
#endif //LAB31PROXY_PARSERTYPEREQUEST_H
