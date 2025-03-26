#include "MemoryManager.h"
#include "MemoryMap.cpp"

/**
 * @brief Construct a new Memory Manager
 * 
 * @param size size of the memory block
 * @param path directory to dump the files with the changes in memory
 */
MemoryManager::MemoryManager(int size, std::string path) {
    this->totalSize = size;
    this->memoryBlock = malloc(size);
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
template<typename T>
void MemoryManager::set(int id, T value) {
    void* ptr = this->map->find(id)->ptr;
    if (ptr != nullptr) {
        *reinterpret_cast<T*>(ptr) = value;
    }
}

/**
 * @brief method to get the value in a memory cell
 * 
 * @tparam T type saved in the cell
 * @param id identifier for the cell
 * @return T value saved in the cell
 */
template<typename T>
T MemoryManager::get(int id) {
    void* ptr = this->map->find(id)->ptr;
    if (ptr != nullptr) {
        return *reinterpret_cast<T*>(ptr);
    }
}

/**
 * @brief method to increase the number of references of a given memory cell
 * 
 * @param id identifier for the cell
 */
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

/**
 * @brief method to decrease the number of references of a given memory cell
 * 
 * @param id identifier for the cell
 */
void MemoryManager::decreaseRefCount(int id) {
    MemoryNode* memNode = this->map->find(id);
    if (memNode != nullptr) {
        memNode->refCount--;
    }
}


void MemoryManager::garbageCollection() {
    while(this->map != nullptr) {
        if (this->map->clean()) {
            this->usedSize = this->map->defragment(this->memoryBlock);
        }
    }
}