#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define NUMBER_OF_SEMAPHORES 2

sem_t sems[NUMBER_OF_SEMAPHORES];

typedef struct argumentsForFunction {
    const char *text;
    int count;
    int start;
} argumentsForFunction;

//int errorCheck(int code, char *inscription) {
//    if (code != SUCCESS) {
//        perror(inscription);
//        return code;
//    }
//    return SUCCESS;
//}

int destroySems(int number) {
    int code = 0;
    for (int i = 0; i < number; i++) {
        code = sem_destroy(&sems[i]);
        if (code != SUCCESS) {
            perror("destroy sems");
            return code;
        }
    }
    return SUCCESS;
}

int initializeSems() {
    for (int i = 0; i < NUMBER_OF_SEMAPHORES; ++i) {
        int code = sem_init(&sems[i], 0, i);
        if (code != SUCCESS) {
            perror("init error");
            destroySems(i);
            return code;
        }
    }
    return SUCCESS;
}

int semaphoreWait(int num) {
    int code = sem_wait(&sems[num]);
    if (code != SUCCESS) {
        perror("wait error");
        return code;
    }
    return SUCCESS;
}

int semaphorePost(int num) {
    int code = sem_post(&sems[num]);
    if (code != SUCCESS) {
        perror("post error");
        return code;
    }
    return SUCCESS;
}

void *printTextInThread(void *args) {
    argumentsForFunction *value = (argumentsForFunction *) args;
    int thisSem = 0;
    int nextSem = 0;

    int code = SUCCESS;
    for (int i = 0; i < value->count; i++) {
        thisSem = value->start;
        nextSem = thisSem + 1;
        code = semaphoreWait(thisSem);
        if (code != SUCCESS) {
            perror("wait error");
            return NULL;
        }

        printf("%s %d\n", value->text, i);

        code = semaphorePost(nextSem);
        if (code != SUCCESS) {
            perror("post error");
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread;
    argumentsForFunction newThread = {"Hello, I'm new thread", 10, 1};
    argumentsForFunction mainThread = {"Hello, I'm main thread", 10, 0};
    int code = initializeSems();
    if (code != SUCCESS) {
        perror("");
        exit(code);
    }

    code = pthread_create(&thread, NULL, printTextInThread, &newThread);
    if (code != SUCCESS) {
        perror("pthread_create error");
        destroySems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    printTextInThread(&mainThread);

    code = pthread_join(thread, NULL);
    if (code != SUCCESS) {
        perror("join error");
        destroySems(NUMBER_OF_SEMAPHORES);
        exit(errno);
    }

    destroySems(NUMBER_OF_SEMAPHORES);
    return SUCCESS;
}