#ifndef MEMORYNODET_H
#define MEMORYNODET_H

#include "MemoryNode.h"

template <typename T>
class MemoryNodeT: public MemoryNode {
    private:
    T* ptr;

    public:
    MemoryNodeT(int id, size_t size, DataType type, void* ptr);
    T* getAddress();
    void setAddress(void* newPtr);
    std::string getValue();
    template <typename U>
    void setValue(U value);
};

#endif