#include "MemoryNode.h"

MemoryNode::MemoryNode(int id, size_t size, DataType type) {
    this->id = id;
    this->size = size;
    this->type = type;
    this->refCount = -1;
    this->next = nullptr;
}

MemoryNode* MemoryNode::getNext() {
    return this->next;
}

void MemoryNode::setNext(MemoryNode* newNode) {
    this->next = newNode;
}

int MemoryNode::getID() {
    return this->id;
}

size_t MemoryNode::getSize() {
    return this->size;
}

DataType MemoryNode::getType() {
    return this->type;
}

int MemoryNode::getRefCount() {
    return this->refCount;
}

void MemoryNode::increaseRefCount() {
    this->refCount++;
}

void MemoryNode::decreaseRefCount() {
    this->refCount--;
}