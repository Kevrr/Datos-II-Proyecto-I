#ifndef MPOINTER_H
#define MPOINTER_H

#include <typeinfo>

template <typename T>
class MPointer {
    private:
    string serverIP;
    int port; 
    int id;

    public:
    MPointer();
    static MPointer<T> New();
    int getID();
    T& operator*();
    template <typename U>
    void operator=(const U& data);
    T& operator&();
    ~MPointer();
};

#endif