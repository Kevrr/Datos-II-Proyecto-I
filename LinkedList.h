#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.cpp"
#include <vector>

class LinkedList {
    private:
    std::vector<Node> nodes;
    MPointer<int> head_index;
    MPointer<int> size;

    public:
    LinkedList();
    ~LinkedList();
    static LinkedList New();
    void clear();
    bool isEmpty();
    int getSize();
    bool cointains(int element);
    void add(int element);
    int remove(int element);
    void print();
};

#endif