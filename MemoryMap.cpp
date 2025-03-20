#include "MemoryMap.h"

MemoryMap::MemoryMap() {
    this->head = nullptr;
    this->size = 0;
}

MemoryMap::~MemoryMap() {
    this->clear();
}

void MemoryMap::clear() {
    while (this->head != nullptr) {
        MemoryNode* temp = this->head;
        this->head = this->head->next;
        delete (temp);
    }
}

int MemoryMap::getSize() {
    return this->size;
}

MemoryNode* MemoryMap::find(int id) {
    MemoryNode* current = this->head;
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
    }
    return nullptr;
}


void MemoryMap::add(int id, size_t size, DataType type, void* ptr) {
    MemoryNode* newNode = new MemoryNode(id, size, type, ptr);
    if (this->head == nullptr) {
        this->head = newNode;
        this->size = 1;
    } else {
        MemoryNode* current = this->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        this->size++;
    }
}

void MemoryMap::clean() {
    MemoryNode* current = this->head;
    MemoryNode* prev = nullptr;

    while (current != nullptr) {
        MemoryNode* temp = nullptr;
        if (current->refCount == 0) {
            temp = current;
            if (prev != nullptr) {
                prev->next = current->next;
            } else {
                this->head = current->next;
            }
        }
        prev = current;
        current = current->next;
        if (temp != nullptr) {
            delete(temp);
            this->size--;
        }
    }
    
}