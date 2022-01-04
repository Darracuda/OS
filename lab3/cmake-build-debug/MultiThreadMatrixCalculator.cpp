//
// Created by Diana Kolpakova on 14.12.2021.
//

#include <thread>
#include <vector>
#include "MultiThreadMatrixCalculator.h"
#include "MatrixCalculator.h"
#include "Distributor.h"

MultiThreadMatrixCalculator::MultiThreadMatrixCalculator(Matrix matrix, int threadsAmount) {
    if (matrix.getSize() < 3) throw std::out_of_range("Matrix.getSize < 3");
    if (threadsAmount < 1) throw std::out_of_range("threadsAmount < 1");

    m_matrix = matrix;
    m_threadsAmount = threadsAmount;
}

int MultiThreadMatrixCalculator::calculateDeterminant() {
    int effectiveThreadAmount = fmin(m_matrix.getSize(), m_threadsAmount);
    std::thread threads[effectiveThreadAmount];
    int results[effectiveThreadAmount];

    Distributor distributor(effectiveThreadAmount, m_matrix.getSize());
    std::vector<int> workLoad = distributor.distribute();

    int currentColumn = 0;
    for (int i = 0; i < effectiveThreadAmount; i++) {
        results[i] = 0;
        int columnAmount = workLoad[i];
        threads[i] = std::thread(calculate, std::ref(m_matrix), currentColumn, columnAmount, std::ref(results[i]));
        currentColumn += columnAmount;
    }

    for (int i = 0; i < effectiveThreadAmount; i++) {
        threads[i].join();
    }

    int result = 0;
    for (int i = 0; i < effectiveThreadAmount; i++) {
        result += results[i];
    }

    return result;
}

void MultiThreadMatrixCalculator::calculate(Matrix &matrix, int startColumn, int columnAmount, int &result) {
    result = 0;
    for (int column = startColumn; column < startColumn + columnAmount; column++) {
        Matrix minor = Matrix::calculateMinorMatrix(matrix, column);
        MatrixCalculator calculator(minor);
        int determinant = calculator.calculateDeterminant();
        int element = matrix.getItem(column, 0);

        if (column % 2 == 0)
            result += element * determinant;
        else
            result -= element * determinant;
    }
}
