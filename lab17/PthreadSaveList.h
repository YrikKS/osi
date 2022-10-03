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
        ElementList* next;
        ElementList* before;
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



#endif //LAB17_PTHREADSAVELIST_H
