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

    public:
    MemoryManager(int size, std::string path);
    int create(size_t size, DataType type);
     // Aquí va la implementación del template
     template<typename T>
     int set(int id, T value) {
         MemoryNode* node = this->map->find(id);
 
         if (node != nullptr) {
             std::cout << "[DEBUG] Set: Encontrado nodo con id: " << id << std::endl;
             std::cout << "[DEBUG] Dirección del nodo: " << node->getAddress() << std::endl;
 
             // Mostrar valor antes de setear
             std::cout << "[DEBUG] Valor antes de setear: " << *reinterpret_cast<T*>(node->getAddress()) << std::endl;
 
             *reinterpret_cast<T*>(node->getAddress()) = value;
 
             // Mostrar valor después de setear
             std::cout << "[DEBUG] Valor después de setear: " << *reinterpret_cast<T*>(node->getAddress()) << std::endl;
 
             this->map->dump();
             return 1;
         } else {
             std::cout << "[DEBUG] Set: No se encontró nodo con id: " << id << std::endl;
             this->map->dump();
             return -1;
         }
     }
    std::string get(int id);
    int increaseRefCount(int id);
    int decreaseRefCount(int id);
    void garbageCollection();
    MemoryNode* getMemoryNodeById(int id);
    ~MemoryManager();
};

#endif