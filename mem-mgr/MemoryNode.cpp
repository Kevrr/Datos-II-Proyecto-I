#include <cstdlib>

enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_UNKNOWN
};

struct MemoryNode {
    int id;
    size_t size;
    DataType type;
    int refCount;
    void* ptr;
    MemoryNode* next;
    MemoryNode (int id, size_t size, DataType type, void* ptr) {
        this->id = id;
        this->size = size;
        this->type = type;
        this->ptr = ptr;
        this->refCount = -1;
        this->next = nullptr;
    }
};
