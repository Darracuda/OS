cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3 main.cpp cmake-build-debug/Matrix.cpp cmake-build-debug/Matrix.h cmake-build-debug/MatrixCalculator.cpp cmake-build-debug/MatrixCalculator.h cmake-build-debug/TimeMeter.cpp cmake-build-debug/TimeMeter.h cmake-build-debug/MultiThreadMatrixCalculator.cpp cmake-build-debug/MultiThreadMatrixCalculator.h cmake-build-debug/Distributor.cpp cmake-build-debug/Distributor.h)