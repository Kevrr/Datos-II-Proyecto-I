#include "mem-mgr/MemoryManager.cpp"

int main() {
    MemoryManager* memmgr = new MemoryManager(1024, "/");
    int id1 = memmgr->create(sizeof(int), TYPE_INT);
    int id2 = memmgr->create(sizeof(float), TYPE_FLOAT);
    int id3 = memmgr->create(sizeof(char), TYPE_CHAR);

    memmgr->set(id1, 42);
    memmgr->set(id2, 3.14f);
    memmgr->set(id3, 'A');

    std::cout << memmgr->get(id1);
    std::cout << memmgr->get(id2);
    std::cout << memmgr->get(id3);

    delete(memmgr);
}