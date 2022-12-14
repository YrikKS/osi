//
// Created by kurya on 10.11.2022.
//

#include <iostream>
#include "BinaryString.h"

using namespace ProxyServer;

BinaryString::BinaryString() {
    data = NULL;
    dataSize = 0;
    mallocedSize = 0;
}

BinaryString::BinaryString(char *otherData, long long int otherLength) {
    dataSize = otherLength;
    if (otherLength > 0) {
        data = new char[otherLength];
        mallocedSize = otherLength;
        for (long long int i = 0; i < otherLength; i++) {
            data[i] = otherData[i];
        }
    } else {
        data = NULL;
    }
}

long long int BinaryString::getLength() {
    return dataSize;
}

std::string BinaryString::toSting() {
    if (dataSize == 0 || dataSize == -1) {
        return std::string("");
    } else {
        std::string newString;
        for (long long int i = 0; i < dataSize; i++) {
            newString += data[i];
        }
        return newString;
    }
//    return std::string(data);
}

char *BinaryString::getData() {
    return data;
}

BinaryString::~BinaryString() {
    if (data != NULL)
        delete[] data;
}

BinaryString::BinaryString(const BinaryString &other) {
    dataSize = other.dataSize;
    data = new char[other.dataSize];
    mallocedSize = other.dataSize;
    for (long long int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
}

BinaryString BinaryString::subBinaryString(long long int positionStart, long long int positionEnd) {
    BinaryString binaryString;
    if (dataSize >= positionEnd - positionStart) {
        binaryString.dataSize = positionEnd - positionStart;
    } else {
        binaryString.dataSize = dataSize;
    }

    binaryString.data = new char[binaryString.dataSize];
    mallocedSize = binaryString.dataSize;

    for (long long int i = 0; i < binaryString.dataSize; i++) {
        binaryString.data[i] = data[i + positionStart];
    }
    return binaryString;
}

BinaryString operator+(BinaryString &other1, BinaryString &other2) {
    long long int size = other1.getLength() + other2.getLength();
    char *buf = new char[size];
//    _binaryString.data = new char[dataSize];
    for (long long int i = 0; i < other1.getLength(); i++) {
        buf[i] = other1.getData()[i];
    }
    for (long long int i = 0; i < other2.getLength(); i++) {
        buf[i + other1.getLength()] = other2.getData()[i];
    }
    BinaryString binaryString(buf, size);
    delete[] buf;
    return binaryString;
}


void BinaryString::add(BinaryString binaryString) {
    long long int newSize = dataSize + binaryString.dataSize;
    char *newData = new char[newSize];
    for (long long int i = 0; i < dataSize; i++) {
        newData[i] = data[i];
    }
    for (long long int i = 0; i < binaryString.getLength(); i++) {
        newData[i + dataSize] = binaryString.getData()[i];
    }
    if (data != NULL) {
        delete[] data;
    }
    data = newData;
    dataSize = newSize;
    mallocedSize = newSize;
}

BinaryString &BinaryString::operator=(const BinaryString &other) {
    delete[] data;
    dataSize = other.dataSize;
    data = new char[dataSize];
    mallocedSize = dataSize;
    for (long long int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

void BinaryString::printer() {
    for (long long int i = 0; i < dataSize; i++) {
        std::cout << data[i];
    }
    std::cout << std::endl;
}

void BinaryString::clearData() {
    for (long long int i = 0; i < mallocedSize; i++)
        data[i] = 0;
    dataSize = 0;
}

void BinaryString::setBinaryString(char *otherData, long long int otherLength) {
    dataSize = otherLength;
//    data = new char[otherLength];
    for (long long int i = 0; i < otherLength; i++) {
        data[i] = otherData[i];
    }
}

void BinaryString::copyData(BinaryString other) {
    dataSize = other.dataSize;
    for (long long int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
}

void BinaryString::setNewDataNotMalloc(BinaryString other, long long int start, long long int end) {
    if (mallocedSize < end - start) {
        delete[] data;
        data = new char[end - start + 10];
        mallocedSize = end - start + 10;
    }
    dataSize = end - start;
    for (long long int i = start; i < end; i++) {
        data[i - start] = other.data[i];
    }
}


void BinaryString::shiftDataNotMalloc(BinaryString other, long long int size) {
    dataSize = dataSize - size;
    for (long long int i = size; i < dataSize; i++) {
        data[i - size] = other.data[i];
    }
}


void BinaryString::setNewDataNotMallocWithPtr(BinaryString* other, long long int start, long long int end) {
//    if (mallocedSize < end - start) {
//        delete[] data;
//        data = new char[end - start + 10];
//        mallocedSize = end - start + 10;
//    }
    dataSize = end - start;
    for (long long int i = start; i < end; i++) {
        data[i - start] = other->data[i];
    }
}


//void BinaryString::setNewDataNotMalloc(BinaryString other, long long int start, long long int end) {
//    if (mallocedSize <= end - start) {
//        delete[] data;
//        data = new char[dataSize + end - start + 10];
//        mallocedSize = dataSize + end - start + 10;
//    }
//    for (long long int i = dataSize; i < dataSize + end - start; i++) {
//        data[i] = other.data[i - dataSize];
//    }
//    dataSize = dataSize + end - start;
//}

void BinaryString::copyAndCreateData(BinaryString other) {
    dataSize = other.dataSize;
    data = new char[dataSize];
    mallocedSize = dataSize;
    for (long long int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
}


/*
 * set new, need delete data before
 */
void BinaryString::mallocNeedSize(long long int size) {
    data = new char[size + 10]; // TODO: check?
    mallocedSize = size + 10;
    dataSize = 0;
}

void BinaryString::addToMallocedBuf(BinaryString other) {
    if (mallocedSize < dataSize + other.dataSize) {
        char *newBuf = new char[dataSize + other.dataSize + 10];

        mallocedSize = dataSize + other.dataSize + 10;
        for (int i = 0; i < dataSize; i++) {
            newBuf[i] = data[i];
        }
        delete[] data;

        for (long long int i = dataSize; i < dataSize + other.dataSize; i++) {
            newBuf[i] = other.data[i - dataSize];
        }
        dataSize = dataSize + other.dataSize;
        data = newBuf;

    } else {
        for (int i = 0; i < other.dataSize; i++) {
            data[i + dataSize] = other.data[i];
        }
        dataSize = dataSize + other.dataSize;
    }
}

void BinaryString::deleteMallocedBuf() {
    delete[] data;
    mallocedSize = 0;
    dataSize = 0;
}

void BinaryString::deleteData() {
    if (data != NULL) {
        delete[] data;
        mallocedSize = 0;
        data = NULL;
        dataSize = 0;
    }
}

long long int BinaryString::getMallocedSize() const {
    return mallocedSize;
}


