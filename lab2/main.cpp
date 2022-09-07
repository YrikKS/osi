#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>

void* childFunc(void* arg) {
    for(int i = 0; i < 10; i++)
        std::cout << "Hello, World! I'm child " << pthread_self() << " " <<  getpid() << std::endl;
    return ((void*)1);
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
        int status_addr;
        pthread_join(pThread, (void**)&status_addr);
        std::cout << "Child end with code: " << status_addr << std::endl;
        std::cout << "I'm have new child " << pThread << std::endl;
        for(int i = 0; i < 10; i++)
            std::cout << "Hello, World! I'm parent " << pthread_self() << std::endl;
    }
    //sleep(1);

    pthread_exit(NULL);
}