#include <stdio.h>
#include <malloc.h>

void basicArray();

void printIntArray(int *array, int arraySize);

void deleteFromArray(int *array, int *arraySize, int deleteIndex);

void filterArray(const int *array, int arraySize, int filterValue, int *filteredArray, int *filteredArraySize);

void arrayExample() {
    basicArray();
}

void basicArray() {
    int arraySize = 10;
    int n[arraySize];

    for (int i = 0; i < arraySize; ++i) {
        n[i] = i * 3;
    }
    printIntArray(n, arraySize);
    deleteFromArray(n, &arraySize, 3);
    printf("After delete: \n");
    printIntArray(n, arraySize);

    int *filteredArray = malloc(arraySize * sizeof(int));
    int filteredArraySize;
    filterArray(n, arraySize, 10, filteredArray, &filteredArraySize);
    printf("Filtered array:\n");
    printIntArray(filteredArray, filteredArraySize);
    free(filteredArray);
}

void printIntArray(int *array, int arraySize) {
    for (int i = 0; i < arraySize; ++i) {
        printf("%i. %i \n", i, array[i]);
    }
}

void deleteFromArray(int *array, int *arraySize, int deleteIndex) {
    for (int i = deleteIndex; i < *arraySize; i++) {
        array[i] = array[i + 1];
    }
    (*arraySize)--;
}

void filterArray(const int *array, int arraySize, int filterValue, int *filteredArray, int *filteredArraySize) {

    *filteredArraySize = 0;
    for (int i = 0; i < arraySize; ++i) {
        if (array[i] > filterValue) {
            filteredArray[(*filteredArraySize)] = array[i];
            (*filteredArraySize)++;
        }
    }
}
