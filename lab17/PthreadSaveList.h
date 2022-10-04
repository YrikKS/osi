//
// Created by kurya on 03.10.2022.
//

#ifndef LAB17_PTHREADSAVELIST_H
#define LAB17_PTHREADSAVELIST_H

#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <cerrno>

template<typename T>
class PthreadSaveList {
    typedef struct ElementList {
        T value;
        ElementList *next;
        ElementList *before;
    } ElementList;
public:
    void addBegin(T element);

    ElementList *head;

    PthreadSaveList();

    void printList();

    void sortList();

    virtual ~PthreadSaveList();

private:
    void lockMutex();

    void unlockMutex();

    pthread_mutex_t mutex_;
};

template<typename T>
void PthreadSaveList<T>::printList() {
    lockMutex();
    ElementList *elementList = head;
    int iteration = 0;
    while (elementList != NULL) {
        if (iteration == 0) {
            std::cout << elementList->value;
        } else {
            std::cout << " - " << elementList->value;
        }
        iteration++;
        elementList = elementList->next;
    }
    std::cout << std::endl;
    unlockMutex();
}

template<typename T>
void PthreadSaveList<T>::addBegin(T element) {
    lockMutex();
    ElementList *newElement = new ElementList;
    ElementList *secondElement = head;
    newElement->value = element;
    newElement->before = NULL;
    newElement->next = secondElement;
    head = newElement;
    unlockMutex();
}

template<typename T>
PthreadSaveList<T>::PthreadSaveList() : head(NULL) {
    pthread_mutex_init(&mutex_, NULL);
}

template<typename T>
void PthreadSaveList<T>::lockMutex() {
    if (pthread_mutex_lock(&mutex_)) {
        perror("Can't lock mutex");
        exit(1);
    }
}

template<typename T>
void PthreadSaveList<T>::unlockMutex() {
    if (pthread_mutex_unlock(&mutex_)) {
        perror("Can't unlock mutex");
        exit(1);
    }
}

template<typename T>
PthreadSaveList<T>::~PthreadSaveList() {
    if (head == NULL) {
    } else if (head->next == NULL) {
        delete head;
    } else {
        ElementList *elementList = head;
        while (elementList->next != NULL) {
            elementList = elementList->next;
            delete elementList->before;
        }
        delete elementList;
    }
    pthread_mutex_destroy(&mutex_);
}

template<typename T>
void PthreadSaveList<T>::sortList() {
    lockMutex();
    int countElement = 0;
    ElementList* elementList = head;
    while (elementList != NULL) {
        countElement++;
        elementList = elementList->next;
    }
    std::cout << countElement << std::endl;
    for (int i = 0; i < countElement; i++) {
        for(int j = 0; j < countElement - i; j++)
    }
    unlockMutex();
}


#endif //LAB17_PTHREADSAVELIST_H
