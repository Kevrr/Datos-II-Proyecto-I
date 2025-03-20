#include "MemoryManager.h"

MemoryManager::MemoryManager(int size) {
    this->memoryBlock = malloc(size);
}

void MemoryManager::create(size_t size, DataType type) {
    if (this->usedSize + size > this->totalSize) {
        return;
    }
    void* ptr = static_cast<char*>(memoryBlock) + usedSize;
    this->usedSize += size;

    this->map->add(this->map->getSize(), size, type, ptr);
}

template<typename T>
void MemoryManager::set(int id, T value) {
    void* ptr = this->map->find(id)->ptr;
    if (ptr != nullptr) {
        *ptr = value;
    }
}

template<typename T>
T MemoryManager::get(int id) {
    void* ptr = this->map->find(id)->ptr;
    if (ptr != nullptr) {
        return *ptr;
    }
}

void MemoryManager::increaseRefCount(int id) {
    MemoryNode* memNode = this->map->find(id);
    if (memNode != nullptr) {
        if (memNode->refCount < 0) {
            memNode->refCount = 1;
        } else {
            memNode->refCount++;
        }
    }
}

void MemoryManager::decreaseRefCount(int id) {
    MemoryNode* memNode = this->map->find(id);
    if (memNode != nullptr) {
        memNode->refCount--;
    }
}