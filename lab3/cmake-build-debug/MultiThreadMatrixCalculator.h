//
// Created by Diana Kolpakova on 14.12.2021.
//

#ifndef LAB3_MULTITHREADMATRIXCALCULATOR_H
#define LAB3_MULTITHREADMATRIXCALCULATOR_H


#include "Matrix.h"

class MultiThreadMatrixCalculator {
private:
    Matrix m_matrix;
    int m_threadsAmount;
    static void calculate(Matrix & matrix, int startColumn, int columnAmount, int & result);
public:
    MultiThreadMatrixCalculator(Matrix matrix, int threadsAmount);
    int calculateDeterminant();
};


#endif //LAB3_MULTITHREADMATRIXCALCULATOR_H
