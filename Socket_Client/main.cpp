#include <iostream>      
#include <cstring>       
#include <winsock2.h>    // Biblioteca para programación de sockets en Windows
#include <ws2tcpip.h>    // Funciones adicionales para sockets en Windows
#pragma comment(lib, "Ws2_32.lib") // Vincula la biblioteca Ws2_32.lib para usar Winsock
using namespace std;

// Función para conectarse a un servidor dado su IP y puerto
SOCKET connectToServer(const string& serverIP, int port) {
    WSADATA wsaData;

    // Inicializa la biblioteca Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Crea un socket para la conexión
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    // Configura la dirección del servidor
    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(serverIP.c_str()); // Convierte la IP a formato de red
    server.sin_family = AF_INET;                         // Familia de direcciones IPv4
    server.sin_port = htons(port);                       // Convierte el puerto a formato de red

    // Intenta conectar al servidor
    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        cout << "Connection failed" << endl; // Muestra un mensaje si la conexión falla
        return INVALID_SOCKET;              // Devuelve un socket inválido
    }

    return sock; // Devuelve el socket conectado
}

// Función para enviar una solicitud al servidor y recibir una respuesta
string sendRequest(string serverIP, int port, const string& request) {
    // Conecta al servidor
    SOCKET sock = connectToServer(serverIP, port);
    if (sock == INVALID_SOCKET) return "Error connecting to server"; // Maneja errores de conexión

    // Envía la solicitud al servidor
    send(sock, request.c_str(), request.length(), 0);

    // Buffer para recibir la respuesta del servidor
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0); // Recibe la respuesta

    // Cierra el socket después de la comunicación
    closesocket(sock);

    // Devuelve la respuesta como una cadena
    return string(buffer);
}

int main() {
    // Dirección IP y puerto del servidor
    string serverIP = "127.0.0.1"; // IP del servidor (localhost)
    int port = 8080;              // Puerto del servidor

    // Envía solicitudes al servidor y muestra las respuestas
    cout << "Server Response: " << sendRequest(serverIP, port, "CREATE 128") << endl; 
    cout << "Server Response: " << sendRequest(serverIP, port, "SET 1 100") << endl;  
    cout << "Server Response: " << sendRequest(serverIP, port, "GET 1") << endl;      
    cout << "Server Response: " << sendRequest(serverIP, port, "INCREASEREFCOUNT 1") << endl; 
    cout << "Server Response: " << sendRequest(serverIP, port, "DECREASEREFCOUNT 1") << endl; 

    // Limpia los recursos de Winsock
    WSACleanup();

    return 0; 
}