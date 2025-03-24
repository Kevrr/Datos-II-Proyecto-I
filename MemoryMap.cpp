#include "MemoryMap.h"
#include <ctime>
#include <fstream>
#include <iostream>

MemoryMap::MemoryMap(std::string path) {
    this->head = nullptr;
    this->dumpPath = path;
    this->files = 1;
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
    this->dump();
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
    } else {
        MemoryNode* current = this->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    this->dump();
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
        }
    }
    this->dump();
}

void MemoryMap::dump() {
    char currentTime[100];
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    strftime(currentTime, sizeof(currentTime), "%B_%d_%Y_%H-%M-%S", timePtr);

    std::string name = currentTime;
    name.append(" (");
    name += std::to_string(this->files);
    name.append(")");
    name.append(".txt");

    std::string outPath = this->dumpPath;
    std::string fileName = outPath.append(name);
    std::ofstream file(fileName);
    if (!file) {
        std::cerr << "couldn't open file " << fileName << std::endl;
        perror("Error details");
        return;
    }
    this->files++;
    file << "Memory Map:\n";
    MemoryNode* temp = head;
    while (temp != nullptr) {
        file << "ID: " << temp->id
             << ", Size: " << temp->size
             << ", Type: " << temp->type
             << ", Address: " << temp->ptr
             << "\n";
        temp = temp->next;
    }
    file.close();
}