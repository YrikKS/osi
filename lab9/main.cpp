#include <iostream>
#include <stdio.h>
#include <cerrno>
#include <stdlib.h>
#include <vector>
#include <pthread.h>
#include <signal.h>

#define num_steps 200000000

bool isEnd = false;
pthread_mutex_t mutex;
int count_iteration_all_thread = 0;
static pthread_barrier_t barrier;

typedef struct Itearation {
    int numIteration;
    int whichToStart;
    double result;
} Itearation;


void *piCalculatuiion(void *arg) {
    double pi = 0.0;
    Itearation *itearation = (Itearation *) arg;
    int count_iteration_each_thread = 0;
    while (!isEnd) {
        for (int i = itearation->whichToStart, j = 0; j < itearation->numIteration; i++, j++) {
            pi += 1.0 / (i * 4.0 + 1.0);
            pi -= 1.0 / (i * 4.0 + 3.0);
        }
        count_iteration_each_thread++;
        itearation->whichToStart += num_steps;
    }
    pthread_mutex_lock(&mutex);
//    std::cout << count_iteration_each_thread << std::endl;
    if (count_iteration_all_thread < count_iteration_each_thread)
        count_iteration_all_thread = count_iteration_each_thread;
    pthread_mutex_unlock(&mutex);

    int status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        pthread_barrier_destroy(&barrier);
    } else if (status != 0) {
        std::cerr << "error wait barrier in thread %d with status = \n" << status << std::endl;
        exit(1); //???
    }
//    std::cout << count_iteration_all_thread << std::endl;
    while (count_iteration_all_thread <= count_iteration_each_thread)
        for (int i = itearation->whichToStart, j = 0; j < itearation->numIteration; i++, j++) {
            pi += 1.0 / (i * 4.0 + 1.0);
            pi -= 1.0 / (i * 4.0 + 3.0);
            count_iteration_each_thread++;
            itearation->whichToStart += num_steps;
        }
    std::cout << count_iteration_each_thread << std::endl;
    itearation->result = pi;
    return (void *)itearation;
}

void listener(int i) {
    isEnd = true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "No input arg" << std::endl;
    }
    int numbThread = atoi(argv[1]);

    pthread_mutex_init(&mutex, NULL);

    int status = pthread_barrier_init(&barrier, NULL, numbThread);
    if (status != 0) {
        std::cerr << "main error: can't init barrier, status\n" << status << std::endl;
        return 1;
    }

    pthread_t *pthreadVector = (pthread_t *) malloc(numbThread * sizeof(pthread_t));
    Itearation *ptrIterationStruct = (Itearation *) malloc(numbThread * sizeof(Itearation));


    int lastIterationNumb = 0;
    for (int i = 0; i < numbThread; i++) {
        ptrIterationStruct[i].whichToStart = lastIterationNumb;
        ptrIterationStruct[i].numIteration = num_steps / numbThread + (num_steps % numbThread - i > 0 ? 1 : 0);
        lastIterationNumb += ptrIterationStruct[i].numIteration;
        ptrIterationStruct[i].result = 0.0;
        if (pthread_create(&pthreadVector[i], NULL, piCalculatuiion, (void *) &ptrIterationStruct[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    double pi = 0.0;
    signal(SIGINT, listener);

    for (int i = 0; i < numbThread; i++) {
        Itearation *itearation;
        pthread_join(pthreadVector[i], (void **) &itearation);
        std::cout << itearation->result << std::endl;
        pi += itearation->result;
    }


    pi = pi * 4.0;
    printf("pi done - %.15g \n", pi);
    free(ptrIterationStruct);
    free(pthreadVector);
    return (EXIT_SUCCESS);
}





