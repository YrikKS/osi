#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>

void *childFunc(void *arg) {
    while (true) {
        pthread_testcancel();
        std::cout << "live" << std::endl;
    }
    return NULL;
}

int main() {
    pthread_t pThread;

    if (pthread_create(&pThread, NULL, childFunc, NULL)) {
        perror("Fail to create process");
        return 0;
    }

    sleep(2);
    int cancelError = pthread_cancel(pThread);
    if (cancelError != 0) {
        std::cout << "Cancel error: ESRCH" << std::endl;
    }

    return 0;
}
// на что ещё заканчивается поток