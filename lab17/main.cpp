#include <iostream>
#include "PthreadSaveList.h"

int main() {
    std::cout << "1" << std::endl;
    PthreadSaveList <int> list;
    list.addBegin(3);
    list.printList();
    list.addBegin(2);
    list.printList();
    return 0;
}
