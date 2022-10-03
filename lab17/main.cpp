#include <iostream>
#include "PthreadSaveList.h"
#include <string.h>

int main() {
    PthreadSaveList<std::string> list;
    list.addBegin("3");
    list.printList();
    list.addBegin("2");
    list.printList();
    return 0;
}
