#include <stdio.h>
#include <stdlib.h>

void printArray(int*, int);
int* sortArrDes(int*, int);
int* sortArrAsc(int*, int);

int main(int argc, char** argv) {

    if(argc != 2){

        printf("Need to supply file to read from.\n");
        return 0;
    }

    FILE* fp = fopen(argv[1], "r");

    if(fp == NULL){
        return 0;
    }

    int size = 0;
    fscanf(fp, " %d ", &size); //fscanf(File pointer variable, type, address of variable to put it in)

    int evenIter = 0;
    int oddIter = 0;
    int evenCount = 0;
    int oddCount = 0;
    int arr[size];

    for(int i = 0; i < size; i++){
        fscanf (fp, "%d", &arr[i]);

        if(arr[i] % 2 == 0){
            evenCount++;
        } else {
            oddCount++;
        }
    }

    int oddArr[oddCount];
    int evenArr[evenCount];

    for(int i = 0; i < size; i++){

        if(arr[i] % 2 == 0){
            evenArr[evenIter] = arr[i];
            evenIter++;
        } else {
            oddArr[oddIter] = arr[i];
            oddIter++;
        }
    }

    sortArrAsc(oddArr, oddCount);
    sortArrDes(evenArr, evenCount);

    int resultArr[size];

    for(int i = 0; i < oddCount; i++){
        resultArr[i] = oddArr[i];
    }
    for(int i = 0; i < evenCount; i++){
        resultArr[oddCount + i] = evenArr[i];
    }

    printf("Original Array: ");
    printArray(arr, size);
    printf("Odd Array: ");
    printArray(oddArr, oddCount);
    printf("Even Array: ");
    printArray(evenArr, evenCount);
    printf("Returned Array: ");
    printArray(resultArr, size);

    fclose(fp);
    return 0;

}

void printArray(int* arr, int size){

    printf("[");

    for(int i = 0; i < size - 1; i++){
        printf("%d, ", arr[i]);
    }

    printf("%d]\n", arr[size - 1]);
}

int* sortArrDes(int* arr, int size){

    for(int i = 0; i < size; i++){
        int max = arr[i];
        int maxIndex = i;
        for(int j = i + 1; j < size; j++){
            if(max < arr[j]){
                max = arr[j];
                maxIndex = j;
            }
        }
        int temp = arr[i];
        arr[i] = max;
        arr[maxIndex] = temp;
    }
    return arr;
}

int* sortArrAsc(int* arr, int size){

    for(int i = 0; i < size; i++){
        int max = arr[i];
        int maxIndex = i;
        for(int j = i + 1; j < size; j++){
            if(max > arr[j]){
                max = arr[j];
                maxIndex = j;
            }
        }
        int temp = arr[i];
        arr[i] = max;
        arr[maxIndex] = temp;
    }

    return arr;

}