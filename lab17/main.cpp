#include <iostream>
#include "PthreadSaveList.h"
#include <unistd.h>

int workСontinues = true;

void *sortFunc(void *arg) {
    PthreadSaveList<std::string> *list = (PthreadSaveList<std::string> *) arg;
    while (workСontinues) {
        sleep(5);
        list->sortList();
    }
}


int main() {
    PthreadSaveList<std::string> list;
    pthread_t pthread;
    int pthreadCreateErrorCode = pthread_create(&pthread, NULL, sortFunc, (void *) &list);
    if (pthreadCreateErrorCode != SUCCSEC) {
        fprintf(stderr, "Error thread create: ", pthreadCreateErrorCode);
        exit(1);
    }

    while (workСontinues) {
        std::string string;
        std::cin >> string;
        if(string == "end") {
            workСontinues = false;
        } else if(string == "\n") {
            list.printList();
        } else {
            list.addBegin(string);
        }
    }

    int pthreadJoinErrorCode = pthread_join(pthread, NULL);
    if(pthreadJoinErrorCode != SUCCSEC) {
        fprintf(stderr, "Error join thread: ", pthreadJoinErrorCode);
        exit(1);
    }
    return 0;
}
