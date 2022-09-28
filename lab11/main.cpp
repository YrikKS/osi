#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>


pthread_mutex_t mutex[3];
bool isReady = 0;

void printer(std::string number_thread, int number_string) {
    std::cout << number_thread << " thread " << number_string << std::endl;
}

void *childFunc(void *arg) {
    pthread_mutex_lock(&mutex[2]);
    isReady = 1;
    int i = 0;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex[i % 3]);
        printer("child", i);
        pthread_mutex_unlock(&mutex[(i + 2) % 3]);
    }
    pthread_mutex_unlock(&mutex[(i + 1) % 3]);

    return ((void *) 0);
}

int main() {
    pthread_t pThread;

    pthread_mutex_init(&(mutex[0]), NULL);
    pthread_mutex_init(&(mutex[1]), NULL);
    pthread_mutex_init(&(mutex[2]), NULL);

    pthread_mutex_lock(&mutex[0]);
    if (pthread_create(&pThread, NULL, childFunc, NULL)) {
        std::cout << "Error: " << std::endl;
        perror("failed to create pThread");
        return 1;
    } else {
        while (!isReady){}
        int i = 0;
        for (i = 0; i < 10; i++) {
            pthread_mutex_lock(&mutex[(i + 1) % 3]);
            printer("parent", i);
            pthread_mutex_unlock(&mutex[i % 3]);
        }
        pthread_mutex_unlock(&mutex[i % 3]);
    }


    pthread_mutex_destroy(&(mutex[0]));
    pthread_mutex_destroy(&(mutex[1]));
    pthread_mutex_destroy(&(mutex[2]));

    pthread_exit(NULL);
}