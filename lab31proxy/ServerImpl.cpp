//
// Created by kurya on 03.11.2022.
//

#include "ServerImpl.h"

using namespace ProxyServer;
void ServerImpl::startServer() {

    while (isWork) {
        int code = poll(pollSet, countClient, TIME_OUT_POLL);

        if(code == -1) {
            perror("poll error");
            //TODO exit
        } else if (code == 0) {
            //??
        } else {
            if(poll_set[0].revents & POLLIN) { // poll sock
                //TODO client connect: create Client + add to pollSet
            }

        }
    }

}
