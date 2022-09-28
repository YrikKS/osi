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
    if (pthread_mutex_lock(&mutex[2]))
        perror("error lock mutex: ");
    isReady = 1;
    int i = 0;
    for (i = 0; i < 10; i++) {
        if (pthread_mutex_lock(&mutex[i % 3]))
            perror("error lock mutex: ");

        printer("child", i);

        if (pthread_mutex_unlock(&mutex[(i + 2) % 3]))
            perror("error unlock mutex: ");
    }
    if (pthread_mutex_unlock(&mutex[i % 3]))
        perror("error unlock mutex: ");

    return ((void *) 0);
}

int main() {
    pthread_t pThread;

    pthread_mutex_init(&(mutex[0]), NULL);
    pthread_mutex_init(&(mutex[1]), NULL);
    pthread_mutex_init(&(mutex[2]), NULL);

    if (pthread_mutex_lock(&mutex[0]))
        perror("error lock mutex: ");
    if (pthread_create(&pThread, NULL, childFunc, NULL)) {
        std::cout << "Error: " << std::endl;
        perror("failed to create pThread");
        return 1;
    } else {
        while (!isReady) {}
        int i = 0;
        for (i = 0; i < 10; i++) {
            if (pthread_mutex_lock(&mutex[(i + 1) % 3]))
                perror("error lock mutex: ");

            printer("parent", i);

            if (pthread_mutex_unlock(&mutex[i % 3]))
                perror("error unlock mutex: ");
        }

        if (pthread_mutex_unlock(&mutex[(i + 1) % 3]))
            perror("error unlock mutex: ");
    }


    pthread_mutex_destroy(&(mutex[0]));
    pthread_mutex_destroy(&(mutex[1]));
    pthread_mutex_destroy(&(mutex[2]));

    pthread_exit(NULL);
}