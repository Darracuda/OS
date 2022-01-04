#include<stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

float (*derivative)(float x, float deltaX) = NULL;
int *(*sort)(int length, int *array) = NULL;

char * realization1Name = "realization1.so";
char * realization2Name = "realization2.so";

typedef enum {
    FIRST_TYPE,
    SECOND_TYPE,
} REALIZATION_TYPE;

void *libraryHandle = NULL;

void loadLibrary(REALIZATION_TYPE realizationType){
    const char *realizationName;
    if(realizationType == FIRST_TYPE){
        realizationName = realization1Name;
    } else{
        realizationName = realization2Name;
    }

    libraryHandle = dlopen(realizationName, RTLD_LAZY);
    if(!libraryHandle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    derivative = dlsym(libraryHandle, "derivative");
    sort = dlsym(libraryHandle, "sort");
    char *err;
    if((err = dlerror())) {
        fprintf(stderr, "%s\n", err);
        exit(EXIT_FAILURE);
    }
}

void unloadLibrary(){
    dlclose(libraryHandle);
}

int main() {
    int command;
    REALIZATION_TYPE currentRealizationType = FIRST_TYPE;
    loadLibrary(currentRealizationType);

    printf("To find derivative Cos(x) - enter '1', then X and DeltaX\n");
    printf("To sort an array - enter '2', then ArrayLength and array elements\n");
    printf("To switch function realization - enter '0'\n");
    printf("Current realization type is %d\n", currentRealizationType == FIRST_TYPE ? 1 : 2);

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
            case 0: {
                if (currentRealizationType == FIRST_TYPE) {
                    currentRealizationType = SECOND_TYPE;
                } else {
                    currentRealizationType = FIRST_TYPE;
                }
                printf("Current realization type is %d\n", currentRealizationType == FIRST_TYPE ? 1 : 2);
                unloadLibrary();
                loadLibrary(currentRealizationType);
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
