cmake_minimum_required(VERSION 3.27)
project(hito2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hito2 main.cpp
        FiboHeap.h
        Node.h
        Node.cpp
        FiboHeap.cpp)
