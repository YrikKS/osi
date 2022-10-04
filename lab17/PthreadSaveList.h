//
// Created by kurya on 03.10.2022.
//

#ifndef LAB17_PTHREADSAVELIST_H
#define LAB17_PTHREADSAVELIST_H

#define SUCCESS 0

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
    ElementList *head;

    void addBegin(T element);

    PthreadSaveList();

    void printList();

    void sortList();

    virtual ~PthreadSaveList();

private:
    void lockMutex();

    int getListLengthNotThreadSave();

    void swapElementNotThreadSave(ElementList *element1, ElementList *element2);

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
    if (secondElement != NULL) {
        secondElement->prev = newElement;
    }
    unlockMutex();
}

template<typename T>
PthreadSaveList<T>::PthreadSaveList() : head(NULL) {
    int mutexInitErrorCode = pthread_mutex_init(&mutex_, NULL);
    if (mutexInitErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex lock: ", mutexInitErrorCode);
        exit(1);
    }
}

template<typename T>
void PthreadSaveList<T>::lockMutex() {
    int mutexLockErrorCode = pthread_mutex_lock(&mutex_);
    if (mutexLockErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex lock: ", mutexLockErrorCode);
        exit(1);
    }
}

template<typename T>
void PthreadSaveList<T>::unlockMutex() {
    int mutexUnLockErrorCode = pthread_mutex_unlock(&mutex_);
    if (mutexUnLockErrorCode != SUCCESS) {
        fprintf(stderr, "Can't unlock mutex: ", mutexUnLockErrorCode);
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

    int mutexDestroyErrorCode = pthread_mutex_destroy(&mutex_);
    if (mutexDestroyErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex destroy: ", mutexDestroyErrorCode);
    }
}


template<typename T>
void PthreadSaveList<T>::sortList() {
    lockMutex();
    int countElement = getListLengthNotThreadSave();
    for (int i = 0; i < countElement; i++) {
        ElementList *elementList = head;

        for (int j = 0; j < countElement - i - 1; j++) {
            if (elementList->value > elementList->next->value) {
                swapElementNotThreadSave(elementList, elementList->next);
            } else {
                elementList = elementList->next;
            }
        }
    }
    unlockMutex();
}

template<typename T>
void PthreadSaveList<T>::swapElementNotThreadSave(ElementList *element1, ElementList *element2) {
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
int PthreadSaveList<T>::getListLengthNotThreadSave() {
    ElementList *elementList = head;
    int countElement = 0;
    while (elementList != NULL) {
        countElement++;
        elementList = elementList->next;
    }
    return countElement;
}


#endif //LAB17_PTHREADSAVELIST_H
