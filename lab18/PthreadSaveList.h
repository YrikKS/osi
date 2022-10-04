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
        pthread_mutex_t mutexElement;
        T value;
        ElementList *prev;
        ElementList *next;
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

    void lockMutexElement(ElementList *elementList);

    void unlockMutexElement(ElementList *elementList);

    void destroyMutexElement(ElementList *elementList);

    void lockForSwap(ElementList *elementList);

    void unlockForSwap(ElementList *elementList);

    int getListLengthNotThreadSave();

    void swapElement(ElementList *element1, ElementList *element2);

    void unlockMutex();


    pthread_mutex_t mutex_;
};

template<typename T>
void PthreadSaveList<T>::printList() {
    int iteration = 0;
    for (ElementList *elementList = head; elementList != NULL; elementList = elementList->next) {
        lockMutexElement(elementList);
        if (iteration == 0) {
            std::cout << elementList->value;
        } else {
            std::cout << " - " << elementList->value;
        }
        iteration++;
        unlockMutexElement(elementList);
    }
    std::cout << std::endl;
}

template<typename T>
void PthreadSaveList<T>::addBegin(T element) {
    ElementList *newElement = new ElementList;
    if (head != NULL)
        lockMutexElement(head);
    ElementList *secondElement = head;
    newElement->value = element;
    newElement->next = secondElement;
    newElement->prev = NULL;
    int mutexInitErrorCode = pthread_mutex_init(&newElement->mutexElement, NULL);
    if (mutexInitErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex lock: ", mutexInitErrorCode);
        exit(1);
    }
    lockMutexElement(newElement);
    head = newElement;
    unlockMutexElement(newElement);

    if (secondElement != NULL) {
        secondElement->prev = newElement;
        unlockMutexElement(secondElement);
    }
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
    for (ElementList *element = head; element != NULL;) {
        destroyMutexElement(element);
        if (element->next != NULL) {
            element = element->next;
            delete element->prev;
        } else {
            delete element;
            break;
        }
    }
    int mutexDestroyErrorCode = pthread_mutex_destroy(&mutex_);
    if (mutexDestroyErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex destroy: ", mutexDestroyErrorCode);
    }
}


template<typename T>
void PthreadSaveList<T>::sortList() {

    lockMutexElement(head);
    ElementList *first = head;
    unlockMutexElement(head);
    for (; first != NULL;) {
        int countSwap = 0;
        for (ElementList *second = first; second != NULL;) {
            if (second->next != NULL) {
                lockForSwap(second);
                if (second->value > second->next->value) {
                    countSwap++;
                    std::cout << countSwap << std::endl;
                    swapElement(second, second->next);
                    unlockForSwap(second->prev);
                } else {
                    unlockForSwap(second);
                    second = second->next;
                }
            } else {
                break;
            }
        }
        if (countSwap == 0) {
            return;
        }
    }
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
int PthreadSaveList<T>::getListLengthNotThreadSave() {
    ElementList *elementList = head;
    int countElement = 0;
    while (elementList != NULL) {
        countElement++;
        elementList = elementList->next;
    }
    return countElement;
}

template<typename T>
void PthreadSaveList<T>::lockMutexElement(PthreadSaveList::ElementList *elementList) {
    int mutexLockErrorCode = pthread_mutex_lock(&elementList->mutexElement);
    if (mutexLockErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex lock: ", mutexLockErrorCode);
        exit(1);
    }
}

template<typename T>
void PthreadSaveList<T>::unlockMutexElement(PthreadSaveList::ElementList *elementList) {
    int mutexUnLockErrorCode = pthread_mutex_unlock(&elementList->mutexElement);
    if (mutexUnLockErrorCode != SUCCESS) {
        fprintf(stderr, "Can't unlock mutex: ", mutexUnLockErrorCode);
        exit(1);
    }
}

template<typename T>
void PthreadSaveList<T>::destroyMutexElement(PthreadSaveList::ElementList *elementList) {
    int mutexDestroyErrorCode = pthread_mutex_destroy(&elementList->mutexElement);
    if (mutexDestroyErrorCode != SUCCESS) {
        fprintf(stderr, "Error mutex destroy: ", mutexDestroyErrorCode);
    }
}

template<typename T>
void PthreadSaveList<T>::lockForSwap(PthreadSaveList::ElementList *elementList) {
    if (elementList->prev != NULL) {
        lockMutexElement(elementList->prev);
    }
    lockMutexElement(elementList);
    lockMutexElement(elementList->next);
    if (elementList->next->next != NULL) {
        lockMutexElement(elementList->next->next);
    }
}

template<typename T>
void PthreadSaveList<T>::unlockForSwap(PthreadSaveList::ElementList *elementList) {
    if (elementList->prev != NULL) {
        unlockMutexElement(elementList->prev);
    }
    unlockMutexElement(elementList);
    unlockMutexElement(elementList->next);
    if (elementList->next->next != NULL)
        unlockMutexElement(elementList->next->next);
}


#endif //LAB17_PTHREADSAVELIST_H
