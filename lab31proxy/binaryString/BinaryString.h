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

        BinaryString(char *otherData, long long int otherLength);

        void setBinaryString(char *otherData, long long int otherLength);

        void deleteData();

        BinaryString(BinaryString const &other);

        BinaryString subBinaryString(long long int positionStart, long long int positionEnd);

        friend BinaryString operator+(BinaryString &other1, BinaryString &other2);

        void add(BinaryString binaryString);

        BinaryString &operator=(const BinaryString &other);

        void clearData();

        void printer();

        ~BinaryString();

        long long int getLength();

        std::string toSting();

        char *getData();

        void copyData(BinaryString other);

        void copyAndCreateData(BinaryString other);

        void mallocNeedSize(long long int size);

        void addToMallocedBuf(BinaryString other);

        void deleteMallocedBuf();

        void setNewDataNotMalloc(BinaryString other, long long int start, long long int end);

        long long int getMallocedSize() const;

        void setNewDataNotMallocWithPtr(BinaryString* other, long long int start, long long int end);

    private:
        long long int dataSize = 0;
        char *data = NULL;

        long long int mallocedSize = 0;
    };
}

#endif //STRINGTEST_BINARYSTRING_H
