//
// Created by kurya on 04.11.2022.
//

#ifndef LAB31PROXY_PARSERTYPEREQUEST_H
#define LAB31PROXY_PARSERTYPEREQUEST_H
namespace ProxyServer {
    typedef enum {
        GET_REQUEST,
        GET_REQUEST_NOT_CASH,
        NOT_GET_REQUEST,
        INVAILD_REQUEST
    } TypeRequest;

    typedef enum {
        END_REQUEST_HEADING,
        END_RESPONESE_HEADING,
        END_REQUEST_BODY,
        END_RESPONESE_BODY,
        NOTHING
    } ResultPars;
}
#endif //LAB31PROXY_PARSERTYPEREQUEST_H
