//
// Created by Diana Kolpakova on 26.12.2021.
// Lab5 - first realization
//
#include <math.h>

float derivative(float x, float deltaX) {
    return (cosf(x + deltaX) - cosf(x))/deltaX;
}

void sort(int length, int *array) {
    // Для всех элементов
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < length - 1 - i; j++) {
            if (array[j + 1] < array[j]) {
                int temp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = temp;
            }
        }
    }
}
