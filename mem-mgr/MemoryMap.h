#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include "MemoryNodeT.h"
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>

class MemoryMap {
    private:
    MemoryNode* head;
    std::string dumpPath;
    int files;
    
    void dump();
    
    public:
    MemoryMap(std::string path);
    void clear();
    MemoryNode* find(int id);
    void add(int id, size_t size, DataType type, void* ptr);
    bool clean();
    size_t defragment(void* startingAddress);
    ~MemoryMap();
};

#endif