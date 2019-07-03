#include <stdio.h>
#include <stdlib.h>

void printMatrix(double** matrix, int row, int col);
void freeMatrix(double** matrix, int row);
void readMatrix(FILE* fp);
double** transposeMatrix(double** matrix, int row, int col);
double** multiplyMatrix(double** m1, double** m2, int row1, int col1, int row2, int col2);
double** matrixX;
double** matrixY;

int rowX, colX, rowY, colY, attributes;

int main(int argc, char** argv) {

    if(argc != 2){
        printf("Error: No File Given.\n");
        return 0;
    }

    FILE* fp = fopen(argv[1], "r");

    readMatrix(fp);

    double** xT = transposeMatrix(matrixX, rowX, colX);
    int colXT = rowX;
    int rowXT = colX;

    double** xSq = multiplyMatrix(xT, matrixY, rowXT, colXT, rowX, colX);

    printf("Matrix X:\n");
    printMatrix(matrixX, rowX, colX);
    printf("Matrix Y:\n");
    printMatrix(matrixY, rowY, colY);
    printf("Matrix X^T:\n");
    printMatrix(xT, rowXT, colXT);
    printf("Matrix X^T * X:\n");

    freeMatrix(matrixX, rowX);
    freeMatrix(matrixY, rowY);
    freeMatrix(xT, colX);
    freeMatrix(xSq, colX);

    fclose(fp);
    return 0;
}

void printMatrix(double** matrix, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void freeMatrix(double** matrix, int row){
    for(int i = 0; i < row; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void readMatrix(FILE* fp){
    fscanf(fp, "%d\n", &attributes);
    fscanf(fp, "%d\n", &rowX);
    colX = attributes + 1;
    colY = 1;
    rowY = rowX;

    matrixX = (double**)malloc(sizeof(double*)*rowX);

    for(int i = 0; i < rowX; i++) {
        matrixX[i] = (double*)malloc(sizeof(double) * colX);
    }

    matrixY = (double**)malloc(sizeof(double*)*rowY);

    for(int i = 0; i < rowY; i++) {
        matrixY[i] = (double*)malloc(sizeof(double) * colY);
    }

    for(int i = 0; i < rowX; i++){
        fscanf(fp, "%lf,", &matrixY[i][0]);
        matrixX[i][0] = 1;
        for(int j = 1; j < colX - 1; j++){
            fscanf(fp, "%lf,", &matrixX[i][j]);
        }
        fscanf(fp, "%lf\n", &matrixX[i][colX - 1]);
    }
}

double** transposeMatrix(double** matrix, int row, int col){

    double** result = (double**)malloc(sizeof(double*)*col);

    for(int i = 0; i < col; i++) {
        result[i] = (double*)malloc(sizeof(double) * row);
    }

    for(int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

double** multiplyMatrix(double** m1, double** m2, int row1, int col1, int row2, int col2){

    if(col1 != row2){
        printf("Error: Matrices cannot be multiplied.\n");
        return m1;
    }

    double** result = (double**)malloc(sizeof(double*)*row1);

    for(int i = 0; i < row1; i++) {
        matrixX[i] = (double*)malloc(sizeof(double) * col2);
    }

    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            for(int k = 0; k < col1; k++){
                result[i][j] = result[i][j] + m1[i][k] + m2[k][j];
            }
        }
    }
    return result;
}