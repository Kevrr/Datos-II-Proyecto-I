#include "LinkedList.h"

LinkedList::LinkedList() {
    head_index = MPointer<int>::New();
    head_index = -1;
    size = MPointer<int>::New();
    size = 0;
}

LinkedList::~LinkedList() {
    clear();
}

LinkedList LinkedList::New() {
    return LinkedList();
}

void LinkedList::clear() {
    nodes.clear();
    head_index = -1;
    size = 0;
}

bool LinkedList::isEmpty() {
    return *size == 0;
}

int LinkedList::getSize() {
    return *size;
}

bool LinkedList::cointains(int element) {
    int index = *head_index;
    while (index != -1) {
        if(*nodes[index].value == element) {
            return true;
        }
        index = *nodes[index].next_index;
    }
    return false;
}

void LinkedList::add(int element) {
    Node newNode = Node(element);
    newNode.next_index = head_index;
    nodes.emplace_back(newNode);
    int newHead = nodes.size() - 1;
    head_index = newHead;
    size = *size + 1;
}
int LinkedList::remove(int element) {
    int index = *head_index;
        int prev_index = -1;

        while (index != -1) {
            if (*nodes[index].value == element) {
                if (prev_index == -1) {
                    head_index = *nodes[index].next_index;
                } else {
                    nodes[prev_index].next_index = nodes[index].next_index;
                }
                size = *size - 1;
                return element;
            }
            prev_index = index;
            index = *nodes[index].next_index;
        }
        return -1; // not found
}
void LinkedList::print() {
    int index = *head_index;
        std::cout << "[ ";
        while (index != -1) {
            std::cout << *nodes[index].value << " ";
            index = *nodes[index].next_index;
        }
        std::cout << "]\n";
}