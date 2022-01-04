//
// Created by Diana Kolpakova on 14.12.2021.
//

#include "MatrixCalculator.h"
#include "Matrix.h"

MatrixCalculator::MatrixCalculator( Matrix matrix) {
    m_matrix = matrix;
}

int MatrixCalculator::calculateDeterminant() {

    if (m_matrix.getSize() == 1){
        int a = m_matrix.getItem(0, 0);
        int result = a;
        return result;
    }

    if (m_matrix.getSize() == 2){
        int a = m_matrix.getItem(0, 0);
        int b = m_matrix.getItem(1, 0);
        int c = m_matrix.getItem(0, 1);
        int d = m_matrix.getItem(1, 1);

        int result = a * d - b * c;
        return result;
    }

    int result = 0;

    for (int column = 0; column < m_matrix.getSize(); column++) {
        Matrix minor = Matrix::calculateMinorMatrix(m_matrix, column);
        MatrixCalculator calculator(minor);
        int determinant = calculator.calculateDeterminant();
        int element = m_matrix.getItem(column, 0);

        if (column % 2 == 0)
            result += element * determinant;
        else
            result -= element * determinant;
    }

    return result;
}
