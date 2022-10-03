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
    while (elementList != NULL) {
        std::cout << elementList->value << " - ";
        elementList = elementList->next;
        if (elementList->next == NULL)
            break;
    }
    std::cout << elementList->value << std::endl;
    unlockMutex();
}

template<typename T>
void PthreadSaveList<T>::addBegin(T element) {
    lockMutex();
    ElementList *newElement = new ElementList;
    head->before = newElement;
    newElement->next = head;
    newElement->value = element;
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
        return;
    }
    if (head->next == NULL) {
        delete head;
    }
    ElementList *elementList = head;
    while (elementList->next != NULL) {
        elementList = elementList->next;
        delete elementList->before;
    }
    delete elementList;
}



#endif //LAB17_PTHREADSAVELIST_H
