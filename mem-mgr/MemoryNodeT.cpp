#include "MemoryNodeT.h"
#include "MemoryNode.cpp"
#include <type_traits>
#include <cstring>

template<typename T>
MemoryNodeT<T>::MemoryNodeT(int id, size_t size, DataType type, void* ptr) : MemoryNode(id, size, type) {
    this->ptr = reinterpret_cast<T*>(ptr); 
}

template<typename T>
void* MemoryNodeT<T>::getAddress() {
    return reinterpret_cast<void*>(this->ptr);
}

template<typename T>
void MemoryNodeT<T>::setAddress(void* newPtr) {
    if (newPtr == nullptr) {
        return;
    }
    if (newPtr != this->ptr) {
        std::memcpy(newPtr, this->ptr, this->getSize());
        this->ptr = reinterpret_cast<T*>(newPtr);
    }
}

template<typename T>
std::string MemoryNodeT<T>::getValue() {
    if (this->ptr != nullptr) {
        T value = *this->ptr;
        if constexpr (std::is_same<T, char>::value) {
            return std::string(1, value);
        }
        return std::to_string(value);
    }
    return "null";
}