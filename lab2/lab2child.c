//
// Created by Diana Kolpakova on 16.12.2021.
//

#include <stdio.h>

int main() {
    int value;
    int summa = 0;

    while (scanf("%d", &value) != EOF)
    {
        summa += value;
    }
    printf("%d",summa);
}

