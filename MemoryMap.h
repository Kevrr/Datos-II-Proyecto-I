#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include "MemoryNode.cpp"

class MemoryMap {
    private:
    MemoryNode* head;
    int size;

    public:
    MemoryMap();
    ~MemoryMap();
    void clear();
    int getSize();
    MemoryNode* find(int id);
    void add(int id, size_t size, DataType type, void* ptr);
    void clean();
};

#endif