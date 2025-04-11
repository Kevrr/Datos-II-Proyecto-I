#include "MemoryManager.h"
#include "MemoryMap.cpp"
#include <unistd.h>

/**
 * @brief Construct a new Memory Manager
 * 
 * @param size size of the memory block
 * @param path directory to dump the files with the changes in memory
 */
MemoryManager::MemoryManager(int size, std::string path) {
    this->totalSize = size;
    this->memoryBlock = malloc(size);
    if (this->memoryBlock == nullptr) {
        throw std::bad_alloc();
    }
    this->usedSize = 0;
    this->currentID = 0;
    this->map = new MemoryMap(path);
}

/**
 * @brief Destroy the Memory Manager, freeing the saved memory and deleting the memory map
 * 
 */
MemoryManager::~MemoryManager() {
    delete(this->map);
    this->map = nullptr;
    free(this->memoryBlock);
}

/**
 * @brief allocates memory for a new value
 * 
 * @param size size of the memory to allocate
 * @param type type for the value
 * @return int ID of the cell for the alocated memory
 */
int MemoryManager::create(size_t size, DataType type) {
    if (this->usedSize + size > this->totalSize) {
        return -1;
    }
    void* ptr = static_cast<char*>(memoryBlock) + usedSize;
    this->usedSize += size;
    
    int id = this->currentID;
    this->currentID++;
    this->map->add(id, size, type, ptr);
    return id;
}

/**
 * @brief method to set a value in a memory cell
 * 
 * @tparam T type saved in the cell
 * @param id identifier for the cell
 * @param value value to save in the address
 */


/**
 * @brief method to get the value in a memory cell
 * 
 * @param id identifier for the cell
 * @return string value saved in the cell
 */
std::string MemoryManager::get(int id) {
    MemoryNode* node = this->map->find(id);
    if (node != nullptr) {
        return node->getValue();
    }
    return "invalid ID";
}

/**
 * @brief method to increase the number of references of a given memory cell
 * 
 * @param id identifier for the cell
 */
int MemoryManager::increaseRefCount(int id) {
    MemoryNode* node = this->map->find(id);
    if (node != nullptr) {
        node->increaseRefCount();
        if (node->getRefCount() == 0) {
            node->increaseRefCount();
        }
        return 1;
    }
    return -1;
}

/**
 * @brief method to decrease the number of references of a given memory cell
 * 
 * @param id identifier for the cell
 */
int MemoryManager::decreaseRefCount(int id) {
    MemoryNode* node = this->map->find(id);
    if (node != nullptr) {
        node->decreaseRefCount();
        return 1;
    }
    return -1;
}

void MemoryManager::garbageCollection() {
    while(this->map != nullptr) {
        if (this->map->clean()) {
            this->usedSize = this->map->defragment(this->memoryBlock);
        }
        usleep(1000);
    }
}

MemoryNode* MemoryManager::getMemoryNodeById(int id) {
    return this->map->find(id);
}

static void* threadEntry(void* arg) {
    MemoryManager* obj = static_cast<MemoryManager*>(arg);
    obj->garbageCollection();
    return nullptr;
}