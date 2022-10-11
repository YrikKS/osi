#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "PthreadSaveList.h"

#define MAX_LENGTH_ONE_WORLD 30
#define MAX_COUNT_WORLDS 100
#define RETURN_SUCCESS (void*) 0
#define TIME_TO_SLEEP_CHILD_FUNC 5


typedef struct {
    char str[MAX_LENGTH_ONE_WORLD];
    int length;
    PthreadSaveList<std::string> *list;
} MyString;

void *addToList(void *arg) {
    MyString *myString = (MyString *) arg;
    sleep(myString->length);
    myString->list->addEnd(myString->str);
    return RETURN_SUCCESS;
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");
    PthreadSaveList<std::string> list;
    MyString *myString = (MyString *) malloc(MAX_COUNT_WORLDS * sizeof(MyString));
    pthread_t pThreads[100];
    int i = 0;
    while (fgets(myString[i].str, 30, file) != NULL) {
        pthread_t pThread;
        myString[i].length = strlen(myString[i].str);
        myString[i].list = &list;
        if (pthread_create(&(pThreads[i]), NULL, addToList, (void *) &myString[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
        i++;
    }

    for(int j = 0; j < i; j++) {
        int pthreadJoinErrorCode = pthread_join(pThreads[j], NULL);
        if(pthreadJoinErrorCode != SUCCESS) {
            perror("Error join thread: ");
            exit(1);
        }
    }
    list.printList();
    fclose(file);
    free(myString);
    pthread_exit(NULL);
}
