#include <iostream>
#include "PthreadSaveList.h"

int main() {
    PthreadSaveList<int> list;
    list.addBegin(3);
    list.printList();
    list.addBegin(4);
    list.printList();
    list.sortList();
    list.printList();
    return 0;
}
