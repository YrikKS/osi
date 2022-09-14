#include <iostream>
#include <stdio.h>
#include <cerrno>
#include <stdlib.h>
#include <vector>
#include <pthread.h>

#define num_steps 200000000

typedef struct {
    int numIteration;
    int whichToStart;
    double result;
} Itearation;


void *piCalculatuiion(void *arg) {
    int pi = 0;
    Itearation *itearation = (Itearation *) arg;
    for (int i = itearation->whichToStart, j = 0; j < itearation->numIteration; i++, j++) {
        pi += 1.0 / (i * 4.0 + 1.0);
        pi -= 1.0 / (i * 4.0 + 3.0);
    }
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

    std::vector<pthread_t> pthreadVector;
    std::vector<Itearation *> ptrIterationStruct;
    ptrIterationStruct.resize(numbThread);
    pthreadVector.resize(numbThread);


    int lastIterationNumb = 0;
    for (int i = 0; i < numbThread; i++) {
        ptrIterationStruct[i] = new Itearation;
        ptrIterationStruct[i]->numIteration = num_steps / numbThread + (num_steps % numbThread - i > 0 ? 1 : 0);
        lastIterationNumb += ptrIterationStruct[i]->numIteration;
        ptrIterationStruct[i]->whichToStart = lastIterationNumb;
        if (pthread_create(pthreadVector[i], NULL, piCalculatuiion, (void *) &ptrIterationStruct[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    double pi = 0;
    for (int i = 0; i < numbThread; i++) {
        Itearation itearation;
        pthread_join(pthreadVector[i], (void **) &itearation);
        pi += itearation.result;
        delete ptrIterationStruct[i];
        //        std::cout << "Child end with code: " << itearation. << std::endl;
    }


    pi = pi * 4.0;
    printf("pi done - %.15g \n", pi);

    return (EXIT_SUCCESS);
}





