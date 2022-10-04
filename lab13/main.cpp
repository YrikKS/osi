#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <cerrno>
#include <unistd.h>

#define RETURN_SUCCESS (void*) 0
#define RETURN_ERROR (void*) 1
#define NUMBER_OF_LINES_TO_PRINT 10
#define SUCCESS 0

pthread_mutex_t mutexOne;
pthread_cond_t condition;
bool threadTurn = false;

typedef struct {
    int numberThread; //1 or 2
    std::string printedLine;
} PrinterSettings;

void printer(std::string numberThread, int number_string) {
    std::cout << numberThread << " " << number_string << std::endl;
}

int mutexLock() {
    int mutexLockErrorCode = pthread_mutex_lock(&mutexOne);
    if (mutexLockErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex lock: ", mutexLockErrorCode);
        return 1;
    }
    return 0;
}

int unlockMutex() {
    int mutexUnLockErrorCode = pthread_mutex_unlock(&mutexOne);
    if (mutexUnLockErrorCode != SUCCESS) {
        fprintf(stderr, "Error unlock mutex: ", mutexUnLockErrorCode);
        return 1;
    }
    return 0;
}

int conditionWait() {
    int condWaitErrorCode = pthread_cond_wait(&condition, &mutexOne);
    if (condWaitErrorCode != SUCCESS) {
        fprintf(stderr, "Error cond wait: ", condWaitErrorCode);
        return 1;
    }
    return 0;
}

int conditionSignal() {
    int condSignalErrorCode = pthread_cond_signal(&condition);
    if (condSignalErrorCode != SUCCESS) {
        fprintf(stderr, "Error cond signal: ", condSignalErrorCode);
        return 1;
    }
    return 0;
}


void *childFunc(void *arg) {
    PrinterSettings *printerSettings = (PrinterSettings *) arg;
    for (int i = 0; i < NUMBER_OF_LINES_TO_PRINT; i++) {
        if (mutexLock() != SUCCESS) {
            return RETURN_ERROR;
        }
        while (threadTurn != printerSettings->numberThread) {
            if (conditionWait() != SUCCESS) {
                return RETURN_ERROR;
            }
        }
        printer(printerSettings->printedLine, i);
        threadTurn = !threadTurn;

        if (unlockMutex() != SUCCESS) {
            return RETURN_ERROR;
        }
        if (conditionSignal() != SUCCESS) {
            return RETURN_ERROR;
        }
    }
    return RETURN_SUCCESS;
}

int main() {
    pthread_t pThread;
    int mutexInitErrorCode = pthread_mutex_init(&mutexOne, NULL);
    if (mutexInitErrorCode != SUCCESS) {
        fprintf(stderr, "Error cond signal: ", mutexInitErrorCode);
        return 1;
    }

    int condInitErrorCode = pthread_cond_init(&condition, NULL);
    if (condInitErrorCode != SUCCESS) {
        fprintf(stderr, "Error create condition: ", condInitErrorCode);
        pthread_mutex_destroy(&mutexOne);
        return 1;
    }

    PrinterSettings settingsForMainThread{0, "Parent thread"};
    PrinterSettings settingsForChildThread{1, "Child thread"};

    int pthreadCreateErrorCode = pthread_create(&pThread, NULL, childFunc, (void *) &settingsForChildThread);
    if (pthreadCreateErrorCode != SUCCESS) {
        fprintf(stderr, "Error to create thread: ", pthreadCreateErrorCode);
        pthread_mutex_destroy(&mutexOne);
        pthread_cond_destroy(&condition);
        return 1;
    } else {
        int childFuncErrorCode = 0;
        childFuncErrorCode = *((int*)childFunc((void *) &settingsForMainThread));
        std::cout << "childFuncErrorCode" << std::endl;
        std::cout << childFuncErrorCode << std::endl;
        if (childFuncErrorCode != SUCCESS) {
            fprintf(stderr, "Error in printer function: ", childFuncErrorCode);
            pthread_mutex_destroy(&mutexOne);
            pthread_cond_destroy(&condition);
            return 1;
        }
    }

    int errorCode = 0;
    int pthreadJoinErrorCode = pthread_join(pThread, (void **) &errorCode);
    if (pthreadJoinErrorCode != SUCCESS) {
        fprintf(stderr, "Error in printer function: ", pthreadJoinErrorCode);
        pthread_mutex_destroy(&mutexOne);
        pthread_cond_destroy(&condition);
        return 1;
    } else if (errorCode != SUCCESS) {
        pthread_mutex_destroy(&mutexOne);
        pthread_cond_destroy(&condition);
        exit(1);
    }

    int mutexDestroyErrorCode = pthread_mutex_destroy(&mutexOne);
    if (mutexDestroyErrorCode != SUCCESS) {
        fprintf(stderr, "Error destroy mutex", pthreadJoinErrorCode);
        pthread_cond_destroy(&condition);
        exit(1);
    }


    int condDestroyErrorCode = pthread_cond_destroy(&condition);
    if (condDestroyErrorCode != SUCCESS) {
        fprintf(stderr, "Erorr destroy condition", condDestroyErrorCode);
        exit(1);
    }

    return 0;
}