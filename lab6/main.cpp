#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>


typedef struct {
    char str[30];
    int length;
} my_string;

void *childFunc(void *arg) {
    my_string *myString = (my_string *) arg;
//    printf("%d\n", myString->length);
    sleep(myString->length);
//    for(int i = 0; i < myString->length; i++)
    printf("%s\n", myString->str);
//    std::cout << myString->str << std::endl;
    return ((void *) 0);
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");

    my_string* myString = (my_string*) malloc(100 * sizeof(my_string));
    int i = 0;
    while (fgets(myString[i].str, 30, file) != NULL) {
        pthread_t pThread;
        myString[i].length = strlen(myString[i].str);
//        printf("%s\n", myString.str);
        if (pthread_create(&pThread, NULL, childFunc, (void *) &myString[i])) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
        i++;
    }
    sleep(20);
    fclose(file);
    pthread_exit(NULL);
}
