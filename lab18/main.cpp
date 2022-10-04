#include <iostream>
#include "PthreadSaveList.h"
#include <unistd.h>

#define RETURN_SUCCESS (void*) 0
#define TIME_TO_SLEEP_CHILD_FUNC 5

bool workContinues = true;

void *sortFunc(void *arg) {
    PthreadSaveList<std::string> *list = (PthreadSaveList<std::string> *) arg;
    while (workContinues) {
        sleep(TIME_TO_SLEEP_CHILD_FUNC);
        list->sortList();
    }
    return RETURN_SUCCESS;
}


int main() {
    PthreadSaveList<std::string> list;
    pthread_t pthread;
    int pthreadCreateErrorCode = pthread_create(&pthread, NULL, sortFunc, (void *) &list);
    if (pthreadCreateErrorCode != SUCCESS) {
        fprintf(stderr, "Error thread create: ", pthreadCreateErrorCode);
        exit(1);
    }

    pthread_t pthread2;
    pthreadCreateErrorCode = pthread_create(&pthread2, NULL, sortFunc, (void *) &list);
    if (pthreadCreateErrorCode != SUCCESS) {
        fprintf(stderr, "Error thread create: ", pthreadCreateErrorCode);
        exit(1);
    }

    while (workContinues) {
        std::string string;
        std::getline(std::cin, string);
        if(string == "end") {
            workContinues = false;
        } else if(string.empty()) {
            list.printList();
        } else {
            list.addBegin(string);
        }
    }

    int pthreadJoinErrorCode = pthread_join(pthread, NULL);
    if(pthreadJoinErrorCode != SUCCESS) {
        fprintf(stderr, "Error join thread: ", pthreadJoinErrorCode);
        exit(1);
    }
    return 0;
}
