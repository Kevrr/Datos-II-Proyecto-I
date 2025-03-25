#include "mem-mgr/MemoryManager.cpp"

int main(int argc, char* argv[]) {
    MemoryManager* memmgr = new MemoryManager(1024, "");
    
    memmgr->run();

    delete(memmgr);
    return 0;
}