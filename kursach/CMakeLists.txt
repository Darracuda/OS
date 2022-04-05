cmake_minimum_required(VERSION 3.21)
project(kursach)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(kursach
        main.cpp
        MKKAllocator.cpp
        MKKAllocator.h
        RMAllocator.cpp
        RMAllocator.h)
