cmake_minimum_required(VERSION 3.29)
project(PA1_DES)

set(CMAKE_CXX_STANDARD 98)

add_executable(PA1_DES main.cpp
        config.h
        DES.cpp
        DES.h)
