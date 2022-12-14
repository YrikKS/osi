#include <iostream>
#include <stdio.h>
#include <cerrno>
#include <stdlib.h>
#include <vector>
#include <pthread.h>

#define num_steps 200000000

typedef struct Itearation {
    int numIteration;
    int whichToStart;
    double result;
} Itearation;


void *piCalculatuiion(void *arg) {
    double pi = 0.0;
    Itearation *itearation = (Itearation *) arg;
    for (int i = itearation->whichToStart, j = 0; j < itearation->numIteration; i++, j++) {
        pi += 1.0 / (i * 4.0 + 1.0);
        pi -= 1.0 / (i * 4.0 + 3.0);
    }
    std::cout << pi << std::endl;
    itearation->result = pi;
    return (void *) itearation;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "No input arg" << std::endl;
    }
    int numbThread = atoi(argv[1]);

    for (int i = 0; i < numbThread; i++)
        std::cout << (num_steps % numbThread - i > 0 ? 1 : 0) << std::endl;

    pthread_t *pthreadVector = (pthread_t *) malloc(numbThread * sizeof(pthread_t));
    Itearation *ptrIterationStruct = (Itearation *) malloc(numbThread * sizeof(Itearation));


    int lastIterationNumb = 0;
    for (int i = 0; i < numbThread; i++) {
        ptrIterationStruct[i].whichToStart = lastIterationNumb;
        ptrIterationStruct[i].numIteration = num_steps / numbThread + (num_steps % numbThread - i > 0 ? 1 : 0);
        lastIterationNumb += ptrIterationStruct[i].numIteration;
        ptrIterationStruct[i].result = 0.0;
        std::cout << "iteration" << lastIterationNumb << std::endl;
        if (pthread_create(&pthreadVector[i], NULL, piCalculatuiion, (void *) &ptrIterationStruct[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    double pi = 0.0;
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





