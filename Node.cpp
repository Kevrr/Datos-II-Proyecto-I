#include "MPointer.cpp"

struct Node {
    MPointer<int> value;
    MPointer<int> next_index;
    Node(int n) {
        value = MPointer<int>::New();
        value = n;
        next_index = MPointer<int>::New();
        next_index = -1;
    }
};
