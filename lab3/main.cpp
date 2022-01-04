#include <iostream>

#include "cmake-build-debug/Matrix.h"
#include "cmake-build-debug/MatrixCalculator.h"
#include "cmake-build-debug/TimeMeter.h"
#include "cmake-build-debug/MultiThreadMatrixCalculator.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Error: incorrect number of arguments" << std::endl;
        std::cout << "Usage: lab3 <numberOfThreads> <matrixFileName>" << std::endl;
        std::cout << "Example: lab3 3 test2.txt" << std::endl;

        return -1;
    }

    int threadsAmount = std::stoi(argv[1]);
    char *filename = argv[2];

    Matrix matrix = Matrix::createFromFile(filename);
    matrix.print();

    MatrixCalculator calculator(matrix);
    TimeMeter timeMeter;
    timeMeter.start();
    int determinant = calculator.calculateDeterminant();
    timeMeter.stop();
    std::cout << "Детерминант = " << determinant << std::endl;
    timeMeter.print();

    MultiThreadMatrixCalculator multiThreadMatrixCalculator(matrix, threadsAmount);
    TimeMeter multiThreadTimeMeter;
    multiThreadTimeMeter.start();
    int multiThreadDeterminant = multiThreadMatrixCalculator.calculateDeterminant();
    multiThreadTimeMeter.stop();
    std::cout << "Детерминант = " << multiThreadDeterminant << std::endl;
    multiThreadTimeMeter.print();

    return 0;
}

