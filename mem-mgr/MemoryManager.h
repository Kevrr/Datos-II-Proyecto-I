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
    std::string dumpPath;

    public:
    MemoryManager(int size, std::string path);
    int create(size_t size, DataType type);
    template<typename T>
    int set(int id, T value);
    template<typename T>
    T get(int id);
    int increaseRefCount(int id);
    int decreaseRefCount(int id);
    ~MemoryManager();
    void run();
};

#endif