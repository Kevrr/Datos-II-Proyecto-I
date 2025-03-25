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
    int getID();
    T& operator*();
    void operator=(const T& data);
    T& operator&();
    ~MPointer();
};

#endif