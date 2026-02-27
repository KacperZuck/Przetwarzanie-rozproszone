cmake_minimum_required(VERSION 3.30)
project(Rozproszone_Lab_5)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rozproszone_Lab_5 klient.cpp
        serwer.cpp
        server.cpp
        server.h
)
