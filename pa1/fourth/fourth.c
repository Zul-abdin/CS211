#include <stdio.h>
#include <stdlib.h>

void printMatrix(float** matrix, int row, int col);
void freeMatrix(float** matrix, int row);
float** readMatrix(FILE* fp);

int main(int argv, char** argc) {

    int row = 3;
    int col = 5;

    float** matrix = (float**)malloc(sizeof(float*)*row);

    for(int i = 0; i < row; i++) {
        matrix[i] = (float *) malloc(sizeof(float) * col);
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            matrix[i][j] = 0;
        }
    }

    printMatrix(matrix, row, col);
    free(matrix);

    FILE* fp = argc[1];
    float** fileMatrix = readMatrix(fp);

    return 0;
}

void printMatrix(float** matrix, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void freeMatric(float** matrix, int row){
    for(int i = 0; i < row; i++){
        free(matrix[i]);
    }
    free(matrix);
}

float** readMatrix(FILE* fp){

}