#include "MPointer.h"
#include "Socket_Client/main.cpp"

template <typename T>
MPointer<T>::MPointer() {
    serverIP = "127.0.0.1";
    port = 9090;
    std::string command = "CREATE ";
    command.append(std::to_string(sizeof(T)));
    std::cout << command << "\n";
    std::string response = sendRequest(serverIP, port, command);
    std::cout << "id = " << response << "\n";
    id = stoi(response);
}

template <typename T>
MPointer<T> MPointer<T>::New() {
    return MPointer<T>();
}

template <typename T>
int MPointer<T>::getID() {
    return id;
}


template <typename T>
T MPointer<T>::operator*() {
    std::string command = "GET ";
    command.append(std::to_string(id));
    std::cout << command << "\n";
    std::string response = sendRequest(serverIP, port, command);
    std::cout << "got " << response << "\n";
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
        value = (response == "true");
    } else if constexpr (std::is_same<T, float>::value) {
        value = std::stof(response);
    } else if constexpr (std::is_same<T, double>::value) {
        value = std::stod(response);
    } 
    return value;
}

template <typename T>
template <typename U>
void MPointer<T>::operator=(const U& data) {
    std::string command;
    
    if constexpr (std::is_convertible<T, std::decay_t<U>>::value) {
        command = "SET ";
        command.append(std::to_string(id));
        command.append(" ");
        std::string value;

        if constexpr (std::is_same<std::decay_t<U>, char>::value) {
            value = std::string(1, data);
        } else if constexpr (std::is_same<std::decay_t<U>, bool>::value) {
            value = data ? "true" : "false";
        } else {
            value = std::to_string(data);
        }

        command.append(value);
        std::cout << command << "\n";
        std::cout << sendRequest(serverIP, port, command) << "\n";
        return;
    } else if constexpr (std::is_convertible<std::decay_t<U>, MPointer<T>>::value) {
        id = data.getID();
        command = "INCREASEREFCOUNT ";
        command.append(std::to_string(id));
        std::cout << command << "\n";
        std::cout << sendRequest(serverIP, port, command) << "\n";
        return;
    }  else {
        static_assert(std::is_same<MPointer<T>, U>::value || std::is_same<T, U>::value, "U must be either MPointer<T> or T");
    }
}


template <typename T>
T MPointer<T>::operator&() {
    std::string command = "GET ";
    command.append(std::to_string(id));
    std::cout << command << "\n";
    std::string response = sendRequest(serverIP, port, command);
    std::cout << "got " << response << "\n";
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
        value = (response == "true");
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