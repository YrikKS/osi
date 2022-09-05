#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>

void* childFunc(void* arg) {
    for(int i = 0; i < 10; i++)
        std::cout << "Hello, World! I'm child " << pthread_self() << " " <<  getpid() << std::endl;
    return ((void*)0);
}

int main() {
    pthread_t pThread;
    std::cout << "Hello, World! I'm not parent now " << pthread_self() << " " << getpid() << std::endl;

    if(pthread_create(&pThread, NULL, childFunc, NULL)) {
        std::cout << "I'm not parent ((( " << pthread_self() << " Error: ";
        perror("faild to create process");
        return 0;
    }
    else {
        std::cout << "I'm have new child " << pThread << std::endl;
        for(int i = 0; i < 10; i++)
            std::cout << "Hello, World! I'm parent " << pthread_self() << std::endl;
    }
    //sleep(1);

    pthread_exit(NULL);
}