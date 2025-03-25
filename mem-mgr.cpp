#include "mem-mgr/MemoryManager.cpp"
#include "Socket_Server/main.cpp"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Uso: ./mem-mgr –port LISTEN_PORT –memsize SIZE_MB –dumpFolder DUMP_FOLDER" << std::endl;
        return 1;
    }

    // Calcula el tamaño del pool de memoria en bytes
    size_t memSize = atoi(argv[4]) * 1024 * 1024;
    MemoryManager* memmgr = new MemoryManager(memSize, argv[6]);
    // Inicializa Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al iniciar Winsock" << std::endl;
        return 1;
    }


    // Crea un hilo para el servidor
    pthread_t serverTid;
    pthread_create(&serverTid, nullptr, serverThread, nullptr);
    pthread_join(serverTid, nullptr); // Espera a que el hilo del servidor termine

    WSACleanup();     // Limpia los recursos de Winsock

    return 0; 
}