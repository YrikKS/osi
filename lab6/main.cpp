#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void *childFunc(void *arg) {
//    char* str = (arg*)arg;
    return ((void *) 0);
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");

    char str[40];
    while (fscanf(file, "%s", str) != EOF) {
        pthread_t pThread;
        std::cout << strlen(str) << std::endl;
        if (pthread_create(&pThread, NULL, childFunc, (void *) &str)) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    fclose(file);
    pthread_exit(NULL);
}
