#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>


pthread_mutex_t mutexOne;
pthread_cond_t condition;
bool threadTurn = 0;

typedef struct {
    int numberThread; //1 or 2
    std::string printedLine;
} PrinterSettings;

void printer(std::string numberThread, int number_string) {
    std::cout << numberThread << " " << number_string << std::endl;
}

void *childFunc(void *arg) {
    PrinterSettings *printerSettings = (PrinterSettings *) arg;
    for (int i = 0; i < 10; i++) {
        if (pthread_mutex_lock(&mutexOne)) {
            return ((void *) 1);
        }
        while (threadTurn != printerSettings->numberThread) {
            if (pthread_cond_wait(&condition, &mutexOne)) {
                return ((void *) 1);
            }
        }
        printer(printerSettings->printedLine, i);
        threadTurn = !threadTurn;
        if (pthread_mutex_unlock(&mutexOne)) {
            return ((void *) 1);
        }
        if (pthread_cond_signal(&condition)) {
            return ((void *) 1);
        }
    }
    return ((void *) 0);
}

int main() {
    pthread_t pThread;

    if (pthread_mutex_init(&mutexOne, NULL)) {
        perror("can't create mutex");
        exit(1);
    }
    if (pthread_cond_init(&condition, NULL)) {
        perror("can't create condition");
        pthread_mutex_destroy(&mutexOne);
        exit(1);
    }

    PrinterSettings settingsForMainThread{0, "Parent thread"};
    PrinterSettings settingsForChildThread{1, "Child thread"};

    if (pthread_create(&pThread, NULL, childFunc, (void *) &settingsForChildThread)) {
        perror("failed to create thread");
        pthread_mutex_destroy(&mutexOne);
        pthread_cond_destroy(&condition);
        exit(1);
    } else {
        if (childFunc((void *) &settingsForMainThread)) {
            perror("failed in printer function");
            pthread_mutex_destroy(&mutexOne);
            pthread_cond_destroy(&condition);
            exit(1);
        }
    }

    int errorCode = 1;
    pthread_join(pThread, (void **) &errorCode);
    std::cout << errorCode << std::endl;
    if (errorCode) {
        perror("failed in printer function");
        pthread_mutex_destroy(&mutexOne);
        pthread_cond_destroy(&condition);
        exit(1);
    }

    if (pthread_mutex_destroy(&mutexOne)) {
        perror("can't destroy mutex");
        pthread_cond_destroy(&condition);
        exit(1);
    }

    if (pthread_cond_destroy(&condition)) {
        perror("can't destroy condition");
        exit(1);
    }

    pthread_exit(NULL);
}