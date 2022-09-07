#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>

typedef struct {
    std::string str;
} string_container;


void *childFunc(void *arg) {
    string_container *str = (string_container *) (arg);
    std::cout << str->str << std::endl;
    return ((void *) 0);
}

void startPthread(std::string string) {
    pthread_t pThread;
    string_container container;
    container.str = string;
    if (pthread_create(&pThread, NULL, childFunc, (void *) &container)) {
        std::cout << "Error: " << std::endl;
        perror("failed to create pThread");
        return;
    } else {
        int status_addr;
        pthread_join(pThread, (void **) &status_addr);
        std::cout << "Child end with code: " << status_addr << std::endl;
    }
    return;
}

int main() {
    startPthread("Hello I'm first");
    startPthread("Hello I'm second");
    startPthread("Hello I'm third");
    startPthread("Hello I'm fourth");

    std::cout << "By, I'm main pthread" << std::endl;
    pthread_exit(NULL);
}