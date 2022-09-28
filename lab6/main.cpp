#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>


typedef struct {
    char *str;
    int length;
} my_string;

void *childFunc(void *arg) {
    my_string *myString = (my_string *) arg;
    printf("%c\n", myString->str[0]);
    sleep(myString->length);
//    std::cout << myString->str << std::endl;
    return ((void *) 0);
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");

    char *str = (char *)malloc(sizeof(char) * 30);
    while (fscanf(file, "%s", str) != EOF) {
        pthread_t pThread;
        my_string myString;
        myString.str = str;
        myString.length = strlen(str);
        if (pthread_create(&pThread, NULL, childFunc, (void *) &myString)) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    fclose(file);
    pthread_exit(NULL);
}
