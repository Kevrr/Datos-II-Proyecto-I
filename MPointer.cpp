#include "MPointer.h"
#include "Socket_Client/main.cpp"

template <typename T>
MPointer<T>::MPointer() {
    this->serverIP = "127.0.0.1";
    this->port = 8080;
    std::string command = "CREATE ";
    command.append(std::to_string(sizeof(T)));
    this->id = sendRequest(serverIP, port, command);
}

template <typename T>
static MPointer<T> MPointer<T>::New() {
    return MPointer<T>();
}

template <typename T>
int MPointer<T>::getID() {
    return this->id;
}


template <typename T>
T& MPointer<T>::operator*() {
    std::string command = "GET ";
    command.append(std::to_string(this->id));
    T value = sendRequest(serverIP, port, "GET 1");
    return value;
}

template <typename T>
template <typename U>
void MPointer<T>::operator=(const U& data) {
    std::string command;
    if (typeid(data) == typeid(MPointer<T>)) {
        this->id = data.getID();
        command = "INCREASEREFCOUNT ";
        command.append(std::to_string(this->id));
        sendRequest(serverIP, port, command)
    } else if (std::is_same<T, U>::data) {
        command = "SET ";
        command.append(std::to_string(this->id));
        command.append(" ");
        std::string value;
        if constexpr (std::is_same<T, char>::data) {
            value = std::string(1, data);
        } else if constexpr (std::is_same<T, bool>::data) {
            value = data ? "true" : "false";
        } else {
            value = std::to_string(data);
        }
        command.append(value);
        sendRequest(serverIP, port, command);
    }
}

template <typename T>
T& MPointer<T>::operator&() {
    std::string command = "GET ";
    command.append(std::to_string(this->id));
    std::string response = sendRequest(serverIP, port, "GET 1");
    if (response.empty()) {
        throw std::invalid_argument("didn't receive response");
    }
    T value;
    if constexpr (std::is_same<T, int>::value) {
        value = std::stoi(response);
    }
    else if constexpr (std::is_same<T, char>::value) {
        value = response[0];
    } else if constexpr (std::is_same<T, bool>::value) {
        value = (str == "true");
    } else if constexpr (std::is_same<T, float>::value) {
        value = std::stof(response);
    } else if constexpr (std::is_same<T, double>::value) {
        value = std::stod(response);
    } 
    return value;
}

template <typename T>
MPointer<T>::~MPointer() {
    std::string command = "DECREASEREFCOUNT ";
    command.append(std::to_string(this->id));
    sendRequest(serverIP, port, command);
}