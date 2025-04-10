#include "MemoryMap.h"
#include "MemoryNodeT.cpp"

/**
 * @brief Construct a new Memory Map
 * 
 * @param path string with the direction where dump files will be created
 */
MemoryMap::MemoryMap(std::string path) {
    this->head = nullptr;
    this->dumpPath = path;
    this->files = 1;
}

/**
 * @brief method that creates a txt with the current state of the memory map
 * 
 */
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
        file << "ID: " << temp->getID()
             << ", Size: " << temp->getSize()
             << ", Type: " << temp->getType()
             << ", Address: " << temp->getAddress()
             << ", Value: " << temp->getValue()
             << "\n";
        temp = temp->getNext();
    }
    file.close();
}

/**
 * @brief method to delete all memory cells in the memory map
 * 
 */
void MemoryMap::clear() {
    while (this->head != nullptr) {
        MemoryNode* temp = this->head;
        this->head = this->head->getNext();
        delete(temp);
    }
    this->dump();
}

/**
 * @brief method to find an memory cell given its ID
 * 
 * @param id 
 * @return MemoryNode* 
 */
MemoryNode* MemoryMap::find(int id) {
    MemoryNode* current = this->head;
    while (current != nullptr) {
        if (current->getID() == id) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

/**
 * @brief method to create an memory cell
 * 
 * @param id ID for the memory cell
 * @param size size of the memory cell
 * @param type type for the value to be saved in the memory cell
 * @param ptr address of the given space
 */
void MemoryMap::add(int id, size_t size, DataType type, void* ptr) {
    MemoryNode* newNode;
    switch (type) {
        case 0: 
        newNode = new MemoryNodeT<int>(id, size, type, ptr);
        case 1:
        newNode = new MemoryNodeT<char>(id, size, type, ptr);
        case 2:
        newNode = new MemoryNodeT<bool>(id, size, type, ptr);
        case 3:
        newNode = new MemoryNodeT<float>(id, size, type, ptr);
        case 4:
        newNode = new MemoryNodeT<double>(id, size, type, ptr);
        default:
        newNode = new MemoryNodeT<void>(id, size, type, ptr);
    }
    
    if (this->head == nullptr) {
        this->head = newNode;
    } else {
        MemoryNode* current = this->head;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(newNode);
    }
    this->dump();
}

/**
 * @brief method that deletes all memory cells with no references
 * 
 */
bool MemoryMap::clean() {
    bool deleted = false;
    MemoryNode* current = this->head;
    MemoryNode* prev = nullptr;

    while (current != nullptr) {
        MemoryNode* temp = nullptr;
        if (current->getRefCount() == 0) {
            temp = current;
            if (prev != nullptr) {
                prev->setNext(current->getNext());
            } else {
                this->head = current->getNext();
            }
            deleted = true;
        }
        prev = current;
        current = current->getNext();
        if (temp != nullptr) {
            delete(temp);
        }
    }
    this->dump();
    return deleted;
}

/**
 * @brief Destroy the Memory Map, clearing all its memory cells first
 * 
 */
MemoryMap::~MemoryMap() {
    this->clear();
}

size_t MemoryMap::defragment(void* startingAddress) {
    size_t currentUsedSize = 0;
    MemoryNode* temp = head;
    while (temp != nullptr) {
        void* newPtr = static_cast<char*>(startingAddress) + currentUsedSize;
        temp->setAddress(newPtr);
        currentUsedSize += temp->getSize();
        temp = temp->getNext();
    }
    return currentUsedSize;
}