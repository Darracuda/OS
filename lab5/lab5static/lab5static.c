#include<stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

extern float derivative(float x, float deltaX);
extern int *sort(int length, int *array);

int main() {
    int command;

    printf("To find derivative Cos(x) - enter '1', then X and DeltaX\n");
    printf("To sort an array - enter '2', then ArrayLength and array elements\n");

    while (scanf("%d", &command) != EOF) {
        switch (command) {
            case 1: {
                float x;
                float deltaX;
                scanf("%f", &x);
                scanf("%f", &deltaX);
                float result = derivative(x, deltaX);
                printf("Result: %f\n", result);
                break;
            }
            case 2: {
                int length;
                scanf("%d", &length);
                int array[length];
                for (int i = 0; i < length; i++)
                    scanf("%d", &array[i]);
                sort(length, array);
                printf("Ordered array: ");
                for (int i = 0; i < length; i++)
                    printf(" %d", array[i]);
                printf("\n");
                break;
            }
            case -1:
                return 0;
            default:
                printf("No option selected");
        }
    }

    return 0;
}
