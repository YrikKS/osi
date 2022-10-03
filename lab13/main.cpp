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
        pthread_mutex_lock(&mutexOne);
        while (threadTurn != printerSettings->numberThread) {
            pthread_cond_wait(&condition, &mutexOne);
        }
        printer(printerSettings->printedLine, i);
        threadTurn = !threadTurn;
        pthread_mutex_unlock(&mutexOne);
        pthread_cond_signal(&condition);
    }
    pthread_mutex_unlock(&mutexOne);
    return ((void *) 0);
}

int main() {
    pthread_t pThread;

    pthread_mutex_init(&mutexOne, NULL);
    pthread_cond_init(&condition, NULL);

    PrinterSettings settingsForMainThread{0, "Parent thread"};
    PrinterSettings settingsForChildThread{1, "Child thread"};
    if (pthread_create(&pThread, NULL, childFunc, (void *) &settingsForChildThread)) {
        std::cout << "Error: " << std::endl;
        perror("failed to create pThread");
        return 1;
    } else {
        childFunc((void *) &settingsForMainThread);
    }


    pthread_mutex_destroy(&mutexOne);
    pthread_cond_destroy(&condition);

    pthread_exit(NULL);
}