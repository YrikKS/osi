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
        ElementList *prev;
    } ElementList;
public:
    void addBegin(T element);

    ElementList *head;

    PthreadSaveList();

    void printList();

    int getListLenghtNotThreadSave();

    void sortList();

    void swapElement(ElementList *element1, ElementList *element2);

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
    newElement->prev = NULL;
    newElement->next = secondElement;
    head = newElement;
    if (secondElement != NULL)
        secondElement->prev = newElement;
//    secondElement.prev = newElement;
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
            delete elementList->prev;
        }
        delete elementList;
    }
    pthread_mutex_destroy(&mutex_);
}


template<typename T>
void PthreadSaveList<T>::sortList() {
    lockMutex();
    int countElement = getListLenghtNotThreadSave();
    for (int i = 0; i < countElement; i++) {
        ElementList *elementList = head;

        for (int j = 0; j < countElement - i - 1; j++) {
            if (elementList->value > elementList->next->value) {
                swapElement(elementList, elementList->next);
            } else {
                elementList = elementList->next;
            }
        }
    }
    unlockMutex();
}

template<typename T>
void PthreadSaveList<T>::swapElement(ElementList *element1, ElementList *element2) {
    if (element1 == head) {
        head = element2;
    }
    element1->next = element2->next;
    element2->prev = element1->prev;
    element1->prev = element2;
    element2->next = element1;
    if (element1->next != NULL) {
        element1->next->prev = element1;
    }
    if (element2->prev != NULL) {
        element2->prev->next = element2;
    }
}

template<typename T>
int PthreadSaveList<T>::getListLenghtNotThreadSave() {
    ElementList *elementList = head;
    int countElement = 0;
    while (elementList != NULL) {
        countElement++;
        elementList = elementList->next;
    }
    return countElement;
}


#endif //LAB17_PTHREADSAVELIST_H
