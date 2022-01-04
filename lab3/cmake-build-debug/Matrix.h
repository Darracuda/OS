//
// Created by Diana Kolpakova on 13.12.2021.
//

#ifndef LAB3_MATRIX_H
#define LAB3_MATRIX_H


class Matrix {
private:
    int m_size;
    int* m_values;
public:
    Matrix(Matrix& matrix);
    Matrix();
    Matrix(int size);
    virtual ~Matrix();
    int getSize() const;
    int getItem(int column, int row);
    void setItem(int column, int row, int value);
    void print();

    static Matrix createEmpty(int size);
    static Matrix createRandom(int size);
    static Matrix createFromFile(char* filename);
    static Matrix calculateMinorMatrix(Matrix matrix, int column);
};


#endif //LAB3_MATRIX_H
