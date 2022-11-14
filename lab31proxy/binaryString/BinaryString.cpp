//
// Created by kurya on 10.11.2022.
//

#include <iostream>
#include "BinaryString.h"

BinaryString::BinaryString() {
    data = NULL;
    dataSize = 0;
}

BinaryString::BinaryString(char *otherData, int otherLength) {
    dataSize = otherLength;
    data = new char[otherLength];
    for (int i = 0; i < otherLength; i++) {
        data[i] = otherData[i];
    }
}

int BinaryString::getLength() {
    return dataSize;
}

std::string BinaryString::toSting() {
    if (dataSize == 0) {
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
        delete data;
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
    binaryString.dataSize = positionEnd - positionStart;
    binaryString.data = new char[binaryString.dataSize];
    for (int i = positionStart; i < positionEnd; i++) {
        binaryString.data[i] = data[i];
    }
    return binaryString;
}

BinaryString operator+(const BinaryString &other1, const BinaryString &other2) {
    BinaryString binaryString;
    binaryString.dataSize = other1.dataSize + other2.dataSize;
    binaryString.data = new char[binaryString.dataSize];
//    binaryString.data = new char[dataSize];
    for (int i = 0; i < other1.dataSize; i++) {
        binaryString.data[i] = other1.data[i];
    }
    for (int i = 0; i < other2.dataSize; i++) {
        binaryString.data[i + other1.dataSize] = other2.data[i];
    }

    return binaryString;
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
    for(int i = 0; i < dataSize; i++)
        data[i] = 0;
}
