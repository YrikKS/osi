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

        friend BinaryString operator+(const BinaryString &other1, const BinaryString &other2);

        BinaryString &operator=(const BinaryString &other);

        void clearData();

        void printer();

        ~BinaryString();

        int getLength();

        std::string toSting();

        char *getData();

    private:
        char *data;
        int dataSize;
    };
}

#endif //STRINGTEST_BINARYSTRING_H
