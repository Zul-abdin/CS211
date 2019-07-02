#include <stdio.h>
#include <stdlib.h>

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

    int arr[size];

    for(int i = 0; i < size; i++){
            fscanf (fp, "%d", &arr[i]);
    }
    printf("[");

    for(int i = 0; i < size - 1; i++){
        printf("%d, ", arr[i]);
    }

    printf("%d]\n", arr[size - 1]);

    return 0;

}