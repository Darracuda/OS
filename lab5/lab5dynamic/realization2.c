//
// Created by Diana Kolpakova on 26.12.2021.
// Lab5 - second realization
//
#include <math.h>

float derivative(float x, float deltaX) {
    return (cosf(x + deltaX) - cosf(x - deltaX)) / (2 * deltaX);
}

void quickSort(int *array, int firstIndex, int lastIndex) {
    int i, j, pivotIndex, temp;

    if (firstIndex < lastIndex) {
        pivotIndex = firstIndex;
        i = firstIndex;
        j = lastIndex;

        while (i < j) {
            while (array[i] <= array[pivotIndex] && i < lastIndex)
                i++;
            while (array[j] > array[pivotIndex])
                j--;
            if (i < j) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }

        temp = array[pivotIndex]; // swap elements
        array[pivotIndex] = array[j];
        array[j] = temp;

        quickSort(array, firstIndex, j - 1);
        quickSort(array, j + 1, lastIndex);

    }
}

int *sort(int length, int *array) {
    quickSort(array, 0, length - 1);
    return array;
}

