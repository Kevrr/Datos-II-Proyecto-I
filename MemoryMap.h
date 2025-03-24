#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include "MemoryNode.cpp"
#include <string>

class MemoryMap {
    private:
    MemoryNode* head;
    std::string dumpPath;
    
    void dump();
    
    public:
    MemoryMap(std::string path);
    ~MemoryMap();
    void clear();
    MemoryNode* find(int id);
    void add(int id, size_t size, DataType type, void* ptr);
    void clean();
};

#endif