#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <winsock2.h>    // Biblioteca para programación de sockets en Windows
#include <ws2tcpip.h>    // Funciones adicionales para sockets en Windows
#include <pthread.h>     // Para manejo de hilos
#include <fstream>
#pragma comment(lib, "Ws2_32.lib") // Vincula la biblioteca Ws2_32.lib para usar Winsock
#include "../mem-mgr/MemoryManager.h"
//#include "../mem-mgr/MemoryManager.cpp"


// Variables globales
size_t memSize;

// Función para crear un bloque de memoria 
int Create(MemoryManager& memoryManager, size_t size, const std::string& type) {
    DataType dataType;

    if (type == "int") {
        dataType = TYPE_INT;
    } else if (type == "float") {
        dataType = TYPE_FLOAT;
    } else if (type == "char") {
        dataType = TYPE_CHAR;
    } else if (type == "double") {
        dataType = TYPE_DOUBLE;
    } else if (type == "bool") {
        dataType = TYPE_BOOL;
    } else {
        return 0; // Retorna error si el tipo no es válido
    }
    std::cout << "[DEBUG] Crear bloque de memoria de tipo: " << type << " y tamaño: " << size << std::endl;

    int res = memoryManager.create(size, dataType);
    return res != -1;     
}

template <typename T>
int Set(MemoryManager& memoryManager, int id, const std::string& valueString) {
    T value; 

    if constexpr (std::is_same<T,int>::value) {
        value = std::stoi(valueString); // Convierte el string a int
    } else if constexpr (std::is_same<T,float>::value) {
        value = std::stof(valueString); // Convierte el string a float
    } else if constexpr (std::is_same<T,char>::value) {
        value = valueString[0]; // Toma el primer carácter del string
    } else if constexpr (std::is_same<T,double>::value){
        value = std::stod(valueString); // Convierte el string a double
    } else if constexpr (std::is_same<T,bool>::value){
        value = (valueString == "true") ? true : false; // Convierte el string a bool
    }else {
        return 0; // Retorna error si el tipo no es válido
    }


    int res = memoryManager.set(id, value);
    return res != -1;
}

template <typename T>
int Get(MemoryManager& memoryManager, int id) {
    /*//T value; // Variable para almacenar el valor obtenido
    T res = memoryManager.get<T>(id); // Elimina el puntero
    if (res != nullptr) {
        //value = *static_cast<T*>(res); // Convierte el puntero a tipo T
        //std::cout << "Valor: " << value << std::endl; // Imprime el valor
        return 1; // Éxito
    }*/
    return 1; // Error
}


int IncreaseRefCount(MemoryManager& memoryManager, int id) {
    int res = memoryManager.increaseRefCount(id);
    return res != -1; 
}

int DecreaseRefCount(MemoryManager& memoryManager, int id) {
    int res = memoryManager.decreaseRefCount(id);
    return res != -1; 
}

// Función para manejar la conexión con un cliente
void* handleClient(void* arg) {
    auto* clientData = (std::pair<int, MemoryManager*>*)arg; // Obtiene el socket y el MemoryManager
    int clientSocket = clientData->first;
    MemoryManager& memoryManager = *(clientData->second);

    char buffer[256] = {0};        // Buffer para recibir datos
    recv(clientSocket, buffer, sizeof(buffer), 0); // Recibe la solicitud del cliente

    std::string request(buffer);  // Convierte el buffer a string
    std::string response;         // Respuesta al cliente
    int result = 0;               // Resultado de la operación
    int value = 0;                // Valor a enviar al cliente

    // Procesa las solicitudes del cliente
    if (request.find("CREATE") == 0) {
        size_t size;
        char type[50];
        sscanf(request.c_str(), "CREATE %zu %s", &size, type); // Extrae los parámetros
        result = Create(memoryManager, size, type);           // Crea el bloque de memoria
    } else if (request.find("SET", 0) == 0) {
        int id;
        char valueBuffer[256];
        sscanf(request.c_str(), "SET %d %s", &id, valueBuffer); // Extrae los parámetros
        std::string valueStr(valueBuffer);

        // Eliminar las comillas si están presentes
        if (valueStr.front() == '\'' && valueStr.back() == '\'') {
            valueStr = valueStr.substr(1, valueStr.size() - 2);
        }

        MemoryNode* node = memoryManager.getMemoryNodeById(id); // Busca el nodo en el administrador de memoria
        DataType type = node->getType(); // Obtiene el tipo de dato del nodo
        
        if (type == TYPE_INT) {
            result = Set<int>(memoryManager, id, valueStr);
        } else if (type == TYPE_FLOAT) {
            result = Set<float>(memoryManager, id, valueStr);
        } else if (type == TYPE_CHAR) {
            result = Set<char>(memoryManager, id, valueStr);
        } else if (type == TYPE_DOUBLE) {
            result = Set<double>(memoryManager, id, valueStr);
        } else if (type == TYPE_BOOL) {
            result = Set<bool>(memoryManager, id, valueStr);
        } else {
            result = 0; // o error
        }
        
    } else if (request.find("GET", 0) == 0) {
        /*int id;
        sscanf(request.c_str(), "GET %d", &id);          // Extrae el parámetro
        result = Get<void*>(memoryManager, id);           // Obtiene el valor del bloque de memoria
        if (result == 1) {
            void* valuePtr = memoryManager.get<void*>(id); // Obtiene el puntero genérico al bloque de memoria
            if (valuePtr != nullptr) {
            //response = "VALUE " + std::to_string(*static_cast<int*>(valuePtr)); // Convierte el valor a int como ejemplo
            result = 1; // Éxito
            } else {
            //response = "ERROR"; // Error si el puntero es nulo
            result = 0;
            }
        }*/
        } else if (request.find("INCREASEREFCOUNT", 0) == 0) {
        int id;
        sscanf(request.c_str(), "INCREASEREFCOUNT %d", &id); // Extrae el parámetro
        value = id;
        result = IncreaseRefCount(memoryManager, id);            // Incrementa el contador de referencias
    } else if (request.find("DECREASEREFCOUNT", 0) == 0) {
        int id;
        sscanf(request.c_str(), "DECREASEREFCOUNT %d", &id); // Extrae el parámetro
        result = DecreaseRefCount(memoryManager, id);       // Decrementa el contador de referencias
    } 

    response = (result == 1) ? "OK" : "ERROR";        // Genera la respuesta

    send(clientSocket, response.c_str(), response.size(), 0); // Envía la respuesta al cliente
    closesocket(clientSocket);                                // Cierra el socket del cliente
    delete clientData;                                        // Libera la memoria del argumento
    return nullptr;
}

// Función para manejar el servidor de sockets
void* serverThread(void* arg) {
    auto* serverData = (std::pair<int, MemoryManager*>*)arg; // Obtiene el puerto y el MemoryManager
    int port = serverData->first;
    MemoryManager& memoryManager = *(serverData->second);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket del servidor
    sockaddr_in serverAddr = {};                       // Configura la dirección del servidor
    serverAddr.sin_family = AF_INET;                   // Familia de direcciones IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;           // Acepta conexiones de cualquier IP
    serverAddr.sin_port = htons(port);                 // Usa el puerto recibido como argumento

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)); // Asocia el socket al puerto
    listen(serverSocket, 5);                                      // Escucha conexiones entrantes

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr); // Acepta una conexión
        auto* clientData = new std::pair<int, MemoryManager*>(clientSocket, &memoryManager); // Prepara los datos del cliente
        pthread_t clientThread;                                   // Crea un hilo para manejar al cliente
        pthread_create(&clientThread, nullptr, handleClient, clientData);
        pthread_detach(clientThread); // Desvincula el hilo para que se limpie automáticamente
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: ./mem-mgr <LISTEN_PORT> <SIZE_MB> <DUMP_FOLDER>" << std::endl;
        return 1;
    }

    // Calcula el tamaño del pool de memoria en bytes
    memSize = atoi(argv[2]) * 1024 * 1024;
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
    pthread_create(&serverTid, nullptr, serverThread, serverData); // Pasa el puerto y el MemoryManager como argumento
    pthread_join(serverTid, nullptr); // Espera a que el hilo del servidor termine

    WSACleanup();     // Limpia los recursos de Winsock

    return 0; 
}
