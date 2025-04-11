#include "MPointer.cpp"
#include "LinkedList.cpp"

int main() {
    MPointer<int> myPtr = MPointer<int>::New();
    myPtr = 42;
    std::cout << *myPtr << std::endl;
    
    LinkedList list = LinkedList::New();
    list.add(1);
    list.add(2);
    list.add(3);
    list.add(4);
    list.add(5);
    list.print();
    list.remove(2);
    list.remove(4);
    list.print();
    list.clear();
    return 0;
}