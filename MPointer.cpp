#include "MPointer.h"
#include "Socket_Client/main.cpp"

template <typename T>
MPointer<T>::MPointer() {
    this->serverIP = "127.0.0.1";
    this->port = 8080;
    this->id = sendRequest(serverIP, port, "CREATE 128")
    sendRequest(serverIP, port, "INCREASEREFCOUNT 1")
}

template <typename T>
int MPointer<T>::getID() {
    return this->id;
}


template <typename T>
T& MPointer<T>::operator*(){
    T value = sendRequest(serverIP, port, "GET 1")
    return value;
}

template <typename T>
template <typename U>
void MPointer<T>::operator=(const U& data) {
    if (typeid(data) == typeid(MPointer<T>*)) {
        this->id = data->getID();
        sendRequest(serverIP, port, "INCREASEREFCOUNT 1")
    } else if (typeid(data) == typeid(value)) {
        sendRequest(serverIP, port, "SET 1 100")
    }
}

template <typename T>
T& MPointer<T>::operator&() {
    T value = sendRequest(serverIP, port, "GET 1")
    return value;
}

template <typename T>
MPointer<T>::~MPointer() {
    sendRequest(serverIP, port, "DECREASEREFCOUNT 1");
}