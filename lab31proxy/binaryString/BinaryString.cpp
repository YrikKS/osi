//
// Created by kurya on 10.11.2022.
//

#include <iostream>
#include "BinaryString.h"

using namespace ProxyServer;

BinaryString::BinaryString() {
    data = NULL;
    dataSize = 0;
}

BinaryString::BinaryString(char *otherData, int otherLength) {
    dataSize = otherLength;
    if (otherLength > 0) {
        data = new char[otherLength];
        for (int i = 0; i < otherLength; i++) {
            data[i] = otherData[i];
        }
    } else {
        data = NULL;
    }
}

int BinaryString::getLength() {
    return dataSize;
}

std::string BinaryString::toSting() {
    if (dataSize == 0 || dataSize == -1) {
        return std::string("");
    } else {
        std::string newString;
        for (int i = 0; i < dataSize; i++) {
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
    for (int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
}

BinaryString BinaryString::subBinaryString(int positionStart, int positionEnd) {
    BinaryString binaryString;
    if (dataSize >= positionEnd - positionStart) {
        binaryString.dataSize = positionEnd - positionStart;
    } else {
        binaryString.dataSize = dataSize;
    }
    binaryString.data = new char[binaryString.dataSize];
    for (int i = 0; i < binaryString.dataSize; i++) {
        binaryString.data[i] = data[i + positionStart];
    }
    return binaryString;
}

BinaryString operator+(BinaryString &other1, BinaryString &other2) {
    int size = other1.getLength() + other2.getLength();
    char *buf = new char[size];
//    _binaryString.data = new char[dataSize];
    for (int i = 0; i < other1.getLength(); i++) {
        buf[i] = other1.getData()[i];
    }
    for (int i = 0; i < other2.getLength(); i++) {
        buf[i + other1.getLength()] = other2.getData()[i];
    }
    BinaryString binaryString(buf, size);
    delete[] buf;
    return binaryString;
}


void BinaryString::add(BinaryString binaryString) {
    int newSize = dataSize + binaryString.dataSize;
    char *newData = new char[newSize];
    for (int i = 0; i < dataSize; i++) {
        newData[i] = data[i];
    }
    for (int i = 0; i < binaryString.getLength(); i++) {
        newData[i + dataSize] = binaryString.getData()[i];
    }
    delete[] data;
    data = newData;
    dataSize = newSize;
}

BinaryString &BinaryString::operator=(const BinaryString &other) {
    delete[] data;
    dataSize = other.dataSize;
    data = new char[dataSize];
    for (int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
    return *this;
}

void BinaryString::printer() {
    for (int i = 0; i < dataSize; i++) {
        std::cout << data[i];
    }
    std::cout << std::endl;
}

void BinaryString::clearData() {
    for (int i = 0; i < dataSize; i++)
        data[i] = 0;
    dataSize = 0;
}

void BinaryString::copyData(BinaryString other) {
    dataSize = other.dataSize;
    for (int i = 0; i < other.dataSize; i++) {
        data[i] = other.data[i];
    }
}


/*
 * set new, need delete data before
 */
void BinaryString::setBinaryString(char *otherData, int otherLength) {
    dataSize = otherLength;
//    data = new char[otherLength];
    for (int i = 0; i < otherLength; i++) {
        data[i] = otherData[i];
    }
}

void BinaryString::deleteData() {
    if (data != NULL) {
        delete[] data;
        data = NULL;
        dataSize = -1;
    }
}


