#include <iostream>
#include "PthreadSaveList.h"

int main() {
    PthreadSaveList<int> list;
    list.addBegin(3);
    list.printList();
    list.addBegin(2);
    list.printList();
    list.sortList();
    return 0;
}
