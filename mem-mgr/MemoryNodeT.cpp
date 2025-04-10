#include "MemoryNodeT.h"
#include "MemoryNode.cpp"
#include <type_traits>

template<typename T>
MemoryNodeT<T>::MemoryNodeT(int id, size_t size, DataType type, void* ptr) : MemoryNode(id, size, type) {
    this->ptr = reinterpret_cast<T*>(ptr); 
}

template<typename T>
T* MemoryNodeT<T>::getAddress() {
    return this->ptr;
}

template<typename T>
void MemoryNodeT<T>::setAddress(void* newPtr) {
    if (newPtr != this->ptr) {
        memmove(newPtr, this->ptr, this->getSize());
        this->ptr = reinterpret_cast<T*>(newPtr);
    }
}

template<typename T>
std::string MemoryNodeT<T>::getValue() {
    if (this->ptr != nullptr) {
        T value = *reinterpret_cast<T*>(this->ptr);
        return std::to_string(value);
    }
}

template<typename T>
template<typename U>
void MemoryNodeT<T>::setValue(U data) {
    if (std::is_same<T, U>::value) {
        *this->ptr = data;
    }
}