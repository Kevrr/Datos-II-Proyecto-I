#include "mem-mgr/MemoryManager.cpp"
#include "Socket_Server/main.cpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: ./mem-mgr -port <LISTEN_PORT> -memsize <SIZE_MB> -dumpFolder <DUMP_FOLDER>" << std::endl;
        return 1;
    }

    // Calcula el tamaño del pool de memoria en bytes
    size_t memSize = atoi(argv[2]) * 1024 * 1024;
    int port = atoi(argv[1]);
    std::string dumpFolder = argv[3];

    // Crea el administrador de memoria
    MemoryManager memoryManager(memSize, dumpFolder);  

    // Inicializa Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al iniciar Winsock" << std::endl;
        return 1;
    }

    // Crea un hilo para el servidor
    auto* serverData = new std::pair<int, MemoryManager*>(port, &memoryManager); // Prepara los datos del servidor
    pthread_t serverTid;
    pthread_t garbageCollector;
    pthread_create(&serverTid, nullptr, serverThread, serverData); // Pasa el puerto y el MemoryManager como argumento
    pthread_create(&garbageCollector, nullptr, threadEntry, &memoryManager);
    pthread_join(serverTid, nullptr); // Espera a que el hilo del servidor termine
    pthread_join(garbageCollector, nullptr);

    WSACleanup();     // Limpia los recursos de Winsock

    return 0; 
}