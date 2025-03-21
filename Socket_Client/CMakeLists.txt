cmake_minimum_required(VERSION 3.30)
project(Socket_Client)

set(CMAKE_CXX_STANDARD 20)

add_executable(Socket_Client main.cpp)

target_link_libraries(Socket_Client PRIVATE ws2_32)
