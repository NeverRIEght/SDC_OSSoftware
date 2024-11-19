#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printPointerValues(void *before, void *after) {
    printf("Pointer before realloc: %p\n", before);
    printf("Pointer after realloc: %p\n", after);
}

double measureReallocTime(void **array, size_t newSize) {
    clock_t start, end;
    start = clock();
    *array = realloc(*array, newSize);
    end = clock();

    if (*array == NULL) {
        fprintf(stderr, "Error: can not allocate memory. Exiting...\n");
        exit(1);
    }

    return (double)(end - start) / CLOCKS_PER_SEC * 1000;
}

void fillArray(char *array, size_t size, char value) {
    for (size_t i = 0; i < size; i++) {
        array[i] = value;
    }
}

size_t getInputSize(char *prompt) {
    size_t size;

    int minSizeMb = 100;
    int maxSizeMb = 1024;

    do {
        printf("%s (in MB, between %d and %d):\n", prompt, minSizeMb, maxSizeMb);
        scanf("%zu", &size);
        if (size < minSizeMb || size > maxSizeMb) {
            printf("Invalid size. Please try again.\n");
        }
    } while (size < minSizeMb || size > maxSizeMb);

    // Convert bytes to megabytes
    return size * 1024 * 1024;
}

int main() {
    size_t sizeA = getInputSize("Enter input size for array A");
    size_t sizeB = getInputSize("Enter input size for array B");

    char *arrayA = malloc(sizeA);
    char *arrayB = malloc(sizeB);

    if (arrayA == NULL || arrayB == NULL) {
        fprintf(stderr, "Error: can not allocate memory. Exiting...\n");
        exit(1);
    }

    fillArray(arrayA, sizeA, 'A');
    fillArray(arrayB, sizeB, 'B');

    printf("Finished creating arrays.\n");

    printf("Increasing arrays sizes...\n");
    void *pointerBeforeA = arrayA;
    double reallocTimeA = measureReallocTime((void **)&arrayA, sizeA * 2);
    printPointerValues(pointerBeforeA, arrayA);
    printf("Time taken to increase A: %.2f ms\n", reallocTimeA);

    void *pointerBeforeB = arrayB;
    double reallocTimeB = measureReallocTime((void **)&arrayB, sizeB * 2);
    printPointerValues(pointerBeforeB, arrayB);
    printf("Time taken to increase B: %.2f ms\n", reallocTimeB);

    printf("Decreasing arrays sizes...\n");
    pointerBeforeA = arrayA;
    reallocTimeA = measureReallocTime((void **)&arrayA, sizeA);
    printPointerValues(pointerBeforeA, arrayA);
    printf("Time taken to decrease A: %.2f ms\n", reallocTimeA);

    pointerBeforeB = arrayB;
    reallocTimeB = measureReallocTime((void **)&arrayB, sizeB);
    printPointerValues(pointerBeforeB, arrayB);
    printf("Time taken to decrease B: %.2f ms\n", reallocTimeB);

    printf("Freeing memory...\n");
    free(arrayA);
    free(arrayB);
    printf("Memory freed.\n");

    printf("Trying to read from freed memory...\n");
    // Unexpected result: segmentation error or random value. It depends on the runtime.
    printf("Value at array[0]: %c\n", arrayA[0]);
    printf("Value at array[1]: %c\n", arrayB[1]);

    // That's why it's important to avoid using pointers after freeing the memory :)
    arrayA = NULL;
    arrayB = NULL;

    return 0;
}
