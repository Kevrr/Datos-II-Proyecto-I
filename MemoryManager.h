#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdlib.h>
#include "MemoryMap.h"

class MemoryManager {
    private:
    size_t totalSize;
    void* memoryBlock;
    size_t usedSize;
    MemoryMap* map;

    void create(size_t size, DataType type);
    template<typename T>
    void set(int id, T value);
    template<typename T>
    T get(int id);
    void increaseRefCount(int id);
    void decreaseRefCount(int id);

    public:
    MemoryManager(int size);
};

#endif