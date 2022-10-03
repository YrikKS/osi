//
// Created by kurya on 03.10.2022.
//

#include "PthreadSaveList.h"

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

