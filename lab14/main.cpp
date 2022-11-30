#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>

#define SUCCESS 0
#define NUMBER_OF_SEMAPHORES 2
#define COUNT_INERATION 10

sem_t sems[NUMBER_OF_SEMAPHORES];

typedef struct argumentsForFunction {
    const char *text;
    int count;
    int start;
} argumentsForFunction;

int destroySems(int number) {
    int code = 0;
    for (int i = 0; i < number; i++) {
        errno = sem_destroy(&sems[i]);
        if (errno != SUCCESS) {
            perror("destroy sems");
            return errno;
        }
    }
    return SUCCESS;
}

int initializeSems() {
    for (int i = 0; i < NUMBER_OF_SEMAPHORES; ++i) {
        errno = sem_init(&sems[i], 0, i);
        if (errno != SUCCESS) {
            perror("init error");
            destroySems(i);
            return errno;
        }
    }
    return SUCCESS;
}

int semaphoreWait(int num) {
    errno = sem_wait(&sems[num]);
    if (errno != SUCCESS) {
        perror("wait error");
        return code;
    }
    return SUCCESS;
}

int semaphorePost(int num) {
    errno = sem_post(&sems[num]);
    if (errno != SUCCESS) {
        perror("post error");
        return errno;
    }
    return SUCCESS;
}

void *printTextInThread(void *args) {
    argumentsForFunction *value = (argumentsForFunction *) args;
    int thisSem = 0;
    int nextSem = 0;
    int code = SUCCESS;
    for (int i = 0; i < value->count; i++) {
        thisSem = (value->start + 1) % NUMBER_OF_SEMAPHORES;
        nextSem = (thisSem + 1) % NUMBER_OF_SEMAPHORES;
        errno = semaphoreWait(thisSem);
        if (errno != SUCCESS) {
            perror("wait error");
            return NULL;
        }

        std::cout << value->text << " " << i << std::endl;
        errno = semaphorePost(nextSem);
        if (errno != SUCCESS) {
            perror("post error");
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread;
    argumentsForFunction mainThread = {"Hello, I'm main thread", COUNT_INERATION, 0};
    argumentsForFunction newThread = {"Hello, I'm new thread", COUNT_INERATION, 1};
    errno = initializeSems();
    if (errno != SUCCESS) {
        perror("");
        exit(errno);
    }

    errno = pthread_create(&thread, NULL, printTextInThread, &newThread);
    if (errno != SUCCESS) {
        perror("pthread_create error");
        destroySems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    printTextInThread(&mainThread);

    errno = pthread_join(thread, NULL);
    if (errno != SUCCESS) {
        perror("join error");
        destroySems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    destroySems(NUMBER_OF_SEMAPHORES);
    return SUCCESS;
}