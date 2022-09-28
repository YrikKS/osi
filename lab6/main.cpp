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
//    printf("%d\n", myString->length);
    sleep(myString->length);
//    for(int i = 0; i < myString->length; i++)
        printf("%s", myString->str);
    printf("\n");
//    std::cout << myString->str << std::endl;
    return ((void *) 0);
}

int main() {
    FILE *file;
    file = fopen("text.txt", "r");

    char *str = (char *)malloc(sizeof(char) * 30);
    while (fgets(str, 30, file) != NULL) {
        pthread_t pThread;
        my_string myString;
        myString.str = str;
        myString.length = strlen(str);
        printf("%s\n", myString.str);
        if (pthread_create(&pThread, NULL, childFunc, (void *) &myString)) {
            std::cout << "Error: " << std::endl;
            perror("failed to create pThread");
            return 1;
        }
    }
    sleep(20);
    fclose(file);
    pthread_exit(NULL);
}
