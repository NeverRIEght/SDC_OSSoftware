#include <stdio.h>
#include <stdlib.h>

void getNumbers(int *array, int count) {
    printf("Input numbers:\n");
    for (int i = 0; i < count; i++) {
        scanf("%d", &array[i]);
    }
}

int getSumOfArray(int *array, int count) {
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += array[i];
    }
    return sum;
}

float getArrayAvg(int sum, int count) {
    return (float)sum / count;
}

int getMinFromArray(int *array, int count) {
    int min = array[0];
    for (int i = 1; i < count; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min;
}

int getMaxFromArray(int *array, int count) {
    int max = array[0];
    for (int i = 1; i < count; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int countEvenNumbers(int *array, int count) {
    int evenNumbers = 0;
    for (int i = 0; i < count; i++) {
        if (array[i] % 2 == 0) {
            evenNumbers++;
        }
    }
    return evenNumbers;
}

int countOddNumbers(int *array, int count) {
    int oddNumbers = 0;
    for (int i = 0; i < count; i++) {
        if (array[i] % 2 != 0) {
            oddNumbers++;
        }
    }
    return oddNumbers;
}

void sortArray(int *array, int count) {
    // bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void printCalcResults(int *numbers, int count) {
    printf("--------------------------------\n");
    printf("input was:\n");
    for (int i = 0; i < count; i++) {
        printf("%d\n", numbers[i]);
    }
    printf("\n");

    int sum = getSumOfArray(numbers, count);
    int evenNumbersCount = countEvenNumbers(numbers, count);
    int oddNumbersCount = countOddNumbers(numbers, count);

    printf("--------------------------------\n");

    printf("sum: %d\n", sum);
    printf("average: %.4f\n", getArrayAvg(sum, count));
    printf("min: %d\n", getMinFromArray(numbers, count));
    printf("max: %d\n", getMaxFromArray(numbers, count));
    printf("even numbers count: %d\n", evenNumbersCount);
    printf("odd numbers count: %d\n", oddNumbersCount);

    printf("--------------------------------\n");

    sortArray(numbers, count);
    printf("Sort result: \n");
    for (int i = 0; i < count; i++) {
        printf("%d\n", numbers[i]);
    }
    printf("\n");
}

int main() {
    int numbersCount;

    printf("Count of numbers:\n");
    scanf("%d", &numbersCount);

    // allocate numbersCount * int size memory for numbers array
    int *numbers = calloc(numbersCount, sizeof(int));

    if (numbers == NULL) {
        printf("Error: can not allocate memory for numbers\n");
        return 1;
    }

    getNumbers(numbers, numbersCount);
    printCalcResults(numbers, numbersCount);

    // free allocated memory
    free(numbers);
    return 0;
}
