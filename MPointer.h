#ifndef MPOINTER_H
#define MPOINTER_H

#include <string>
#include <typeinfo>

template <typename T>
class MPointer {
    private:
    std::string serverIP;
    int port; 
    int id;

    public:
    MPointer();
    static MPointer<T> New();
    int getID();
    T operator*();
    template <typename U>
    void operator=(const U& data);
    T operator&();
    ~MPointer();
};

#endif