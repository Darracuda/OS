//
// Created by Diana Kolpakova on 13.12.2021.
//

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Matrix.h"

#define MAX_ITEM_VALUE 10

Matrix::Matrix(Matrix& matrix)
{
    m_size = matrix.m_size;
    m_values = new int[m_size * m_size];
    for (int i = 0; i < m_size * m_size; i++)
        m_values[i] = matrix.m_values[i];
}

Matrix::Matrix() {
    m_size = 0;
    m_values = new int[0];
}

Matrix::Matrix(int size) {
    m_size = size;
    m_values = new int[size * size];
}

Matrix::~Matrix() {
 //   delete[] m_values;
}

int Matrix::getSize() const {
    return m_size;
}

int Matrix::getItem(int column, int row) {
    if ((column < 0) || (column >= m_size)) throw std::out_of_range("column");
    if ((row < 0) || (row >= m_size)) throw std::out_of_range("row");

    return m_values[row * m_size + column];
}

void Matrix::setItem(int column, int row, int value) {
    if ((column < 0) || (column >= m_size)) throw std::out_of_range("column");
    if ((row < 0) || (row >= m_size)) throw std::out_of_range("row");

    m_values[row * m_size + column] = value;
}

void Matrix::print() {
    std::cout << "------- matrix " << m_size << "*" << m_size << " -------" << std::endl;
    for (int row = 0; row < m_size; row++) {
        for (int column = 0; column < m_size; column++) {
            std::cout << std::setw(6) << getItem(column, row);
        }
        std::cout << std::endl;
    }

    std::cout << "---------------------------" << std::endl;
}

Matrix Matrix::createEmpty(int size) {
    Matrix matrix(size);

    for (int row = 0; row < size; row++)
        for (int column = 0; column < size; column++)
            matrix.setItem(column, row, 0);

    return matrix;
}

Matrix Matrix::createRandom(int size) {
    Matrix matrix(size);

    for (int row = 0; row < size; row++)
        for (int column = 0; column < size; column++)
            matrix.setItem(column, row, rand() % MAX_ITEM_VALUE - (MAX_ITEM_VALUE / 2));

    return matrix;
}

Matrix Matrix::createFromFile(char *filename) {
    std::ifstream input(filename);
    int size = 0;
    input >> size;

    Matrix matrix(size);

    for (int row = 0; row < size; row++)
        for (int column = 0; column < size; column++) {
            int value = 0;
            input >> value;
            matrix.setItem(column, row, value);
        }

    return matrix;
}

Matrix Matrix::calculateMinorMatrix(Matrix matrix, int strikeOutColumn) {

    if (matrix.getSize() < 3) throw std::out_of_range("size");
    if (strikeOutColumn > matrix.getSize() || strikeOutColumn < 0) throw std::out_of_range("strikeOutColumn");

    Matrix minor(matrix.getSize()-1);

    for (int row = 1; row < matrix.getSize(); row++) {
        for (int column = 0; column < strikeOutColumn; column++) {
            int value = matrix.getItem(column, row);
            minor.setItem(column, row - 1, value);
        }
        for (int column = strikeOutColumn + 1; column < matrix.getSize(); column++) {
            int value = matrix.getItem(column, row);
            minor.setItem(column - 1, row - 1, value);
        }
    }

    return minor;
}
