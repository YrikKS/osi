//
// Created by kurya on 10.11.2022.
//

#ifndef STRINGTEST_BINARYSTRING_H
#define STRINGTEST_BINARYSTRING_H

#include <string>
#include <cstring>
#include <cctype>

namespace ProxyServer {
    class BinaryString {
    public:
        BinaryString();

        BinaryString(char *otherData, int otherLength);

        void setBinaryString(char *otherData, int otherLength);

        void deleteData();

        BinaryString(BinaryString const &other);

        BinaryString subBinaryString(int positionStart, int positionEnd);

        friend BinaryString operator+(BinaryString &other1, BinaryString &other2);

        void add(BinaryString binaryString);

        BinaryString &operator=(const BinaryString &other);

        void clearData();

        void printer();

        ~BinaryString();

        int getLength();

        std::string toSting();

        char *getData();

        void copyData(BinaryString other);

        void copyAndCreateData(BinaryString other);

    private:
        int dataSize = 0;
        char *data = NULL;


    };
}

#endif //STRINGTEST_BINARYSTRING_H
