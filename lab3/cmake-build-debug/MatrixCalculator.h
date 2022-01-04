//
// Created by Diana Kolpakova on 14.12.2021.
//

#ifndef LAB3_MATRIXCALCULATOR_H
#define LAB3_MATRIXCALCULATOR_H


#include "Matrix.h"

class MatrixCalculator {
private:
    Matrix m_matrix;
public:
    MatrixCalculator(Matrix matrix);

    int calculateDeterminant();
};


#endif //LAB3_MATRIXCALCULATOR_H
