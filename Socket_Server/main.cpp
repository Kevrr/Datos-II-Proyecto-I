#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <winsock2.h>    // Biblioteca para programación de sockets en Windows
#include <ws2tcpip.h>    // Funciones adicionales para sockets en Windows
#include <pthread.h>     // Para manejo de hilos
#include <fstream>
#pragma comment(lib, "Ws2_32.lib") // Vincula la biblioteca Ws2_32.lib para usar Winsock


// Variables globales
size_t memSize;

// Función para crear un bloque de memoria 
int Create(size_t size, const std::string& type) {
    return 1;        
}

int Set(int id, const std::string& value) {
    return 1;
}

int Get(int id) {
    return 1;
}

int IncreaseRefCount(int id) {
    return 1;
}

int DecreaseRefCount(int id) {
    return 1;
}

// Función para manejar la conexión con un cliente
void* handleClient(void* arg) {
    int clientSocket = *(int*)arg; // Obtiene el socket del cliente
    char buffer[256] = {0};        // Buffer para recibir datos
    recv(clientSocket, buffer, sizeof(buffer), 0); // Recibe la solicitud del cliente

    std::string request(buffer);  // Convierte el buffer a string
    std::string response;         // Respuesta al cliente
    int result = 0;               // Resultado de la operación

    // Procesa las solicitudes del cliente
    if (request.find("CREATE") == 0) {
        size_t size;
        char type[50];
        sscanf(request.c_str(), "CREATE %zu %s", &size, type); // Extrae los parámetros
        result = Create(size, type);                          // Crea el bloque de memoria
    } else if (request.find("SET", 0) == 0) {
        int id;
        char value[50];
        sscanf(request.c_str(), "SET %d %s", &id, value); // Extrae los parámetros
        result = Set(id, value);                      // Establece el valor del bloque de memoria
    } else if (request.find("GET", 0) == 0) {
        int id;
        sscanf(request.c_str(), "GET %d", &id); // Extrae el parámetro
        result = Get(id);                   // Obtiene el valor del bloque de memoria
    } else if (request.find("INCREASEREFCOUNT", 0) == 0) {
        int id;
        sscanf(request.c_str(), "INCREASEREFCOUNT %d", &id); // Extrae el parámetro
        result = IncreaseRefCount(id);                // Incrementa el contador de referencias
    } else if (request.find("DECREASEREFCOUNT", 0) == 0) {
        int id;
        sscanf(request.c_str(), "DECREASEREFCOUNT %d", &id); // Extrae el parámetro
        result = DecreaseRefCount(id);                // Decrementa el contador de referencias
    } 

    response = (result == 1) ? "OK" : "ERROR";        // Genera la respuesta

    send(clientSocket, response.c_str(), response.size(), 0); // Envía la respuesta al cliente
    closesocket(clientSocket);                                // Cierra el socket del cliente
    return nullptr;
}

// Función para manejar el servidor de sockets
void* serverThread(void* arg) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket del servidor
    sockaddr_in serverAddr = {};                       // Configura la dirección del servidor
    serverAddr.sin_family = AF_INET;                   // Familia de direcciones IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;           // Acepta conexiones de cualquier IP
    serverAddr.sin_port = htons(8080);                 // Puerto del servidor

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)); // Asocia el socket al puerto
    listen(serverSocket, 5);                                      // Escucha conexiones entrantes

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr); // Acepta una conexión
        pthread_t clientThread;                                   // Crea un hilo para manejar al cliente
        pthread_create(&clientThread, nullptr, handleClient, &clientSocket);
        pthread_detach(clientThread); // Desvincula el hilo para que se limpie automáticamente
    }
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        std::cerr << "Uso: ./mem-mgr <SIZE_MB>" << std::endl;
        return 1;
    }

    // Calcula el tamaño del pool de memoria en bytes
    memSize = atoi(argv[1]) * 1024 * 1024;

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
