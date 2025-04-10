#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdlib.h>
#include "MemoryMap.h"
#include <string>

class MemoryManager {
    private:
    size_t totalSize;
    void* memoryBlock;
    size_t usedSize;
    int currentID;
    MemoryMap* map;

    public:
    MemoryManager(int size, std::string path);
    ~MemoryManager();
    int create(size_t size, DataType type);
    template<typename T>
    void set(int id, T value);
    std::string get(int id);
    void increaseRefCount(int id);
    void decreaseRefCount(int id);
    void garbageCollection();
    ~MemoryManager();
};

#endif