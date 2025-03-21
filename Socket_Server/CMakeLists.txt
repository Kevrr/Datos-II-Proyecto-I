cmake_minimum_required(VERSION 3.30)
project(Socket_Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(Socket_Server main.cpp)

target_link_libraries(Socket_Server PRIVATE ws2_32)
