cmake_minimum_required(VERSION 3.15)
project(testSocket1)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32)

add_executable(testSocketServer main.cpp)

add_executable(testSocketClient client.cpp)