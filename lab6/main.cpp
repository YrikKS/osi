#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>


#define MAX_LENGTH_ONE_WORLD 30
#define MAX_COUNT_WORLDS 100

typedef struct {
    char str[MAX_LENGTH_ONE_WORLD];
    int length;
} MyString;

void *childFunc(void *arg) {
    auto *myString = (MyString *) arg;
    sleep(myString->length);
    printf("%s\n", myString->str);
    return ((void *) 0);
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");

    auto *myString = (MyString *) malloc(MAX_COUNT_WORLDS * sizeof(MyString));
    int i = 0;
    while (fgets(myString[i].str, 30, file) != nullptr) {
        pthread_t pThread;
        myString[i].length = strlen(myString[i].str);
        if (pthread_create(&pThread, nullptr, childFunc, (void *) &myString[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
        i++;
    }
    fclose(file);
    free(myString);
    pthread_exit(nullptr);
}
