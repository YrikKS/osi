#include <iostream>
#include <pthread.h>
#include <unistd.h>

void childFunc() {
    std::cout << "Hello, World! I'm child" << pthread_self() << std::endl;
}

int main() {
    pthread_t pThread;

    if(!pthread_create(*pThread, NULL, childFunc(), NULL)) {
        std::cout << "I'm parent" << pthread_self() << " Error: " << errno << std::endl;
    }
    else {
        std::cout << "Hello, World! I'm parent" << pthread_self() << std::endl;
    }
    sleep(1);

    return 0;
}