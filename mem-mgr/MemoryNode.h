#ifndef MEMORYNODE_H
#define MEMORYNODE_H

#include <string>

enum DataType {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_DOUBLE
};

class MemoryNode {
    private:
    int id;
    size_t size;
    DataType type;
    int refCount;
    MemoryNode* next;
    
    public:
    MemoryNode(int id, size_t size, DataType type);
    MemoryNode* getNext();
    void setNext(MemoryNode* newNode);
    int getID();
    size_t getSize();
    DataType getType();
    int getRefCount();
    void increaseRefCount();
    void decreaseRefCount();
    virtual void* getAddress() = 0;
    virtual void setAddress(void* newPtr) = 0;
    virtual std::string getValue() = 0;
};

#endif