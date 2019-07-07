#include <stdio.h>
#include <stdlib.h>

void printMatrix(double** matrix, int row, int col);
void freeMatrix(double** matrix, int row);
double** readMatrix(FILE* fp, int row, int col);
double** readMatrixTest(FILE* fp, int row, int col);
double** createMatrixX(double** matrix, int row, int col);
double** createMatrixY(double** matrix, int row);
double** transposeMatrix(double** matrix, int row, int col);
double** multiplyMatrix(double** m1, double** m2, int row1, int col1, int row2, int col2);
double** invertMatrix(double** matrix, int row);

int main(int argc, char** argv) {

    int rowX, colX, rowY, colY, attributes;

    if(argc != 3){
        printf("Error: No File Given.\n");
        return 0;
    }

    FILE* fp = fopen(argv[1], "r");

    fscanf(fp, "%d\n", &attributes);
    fscanf(fp, "%d\n", &rowX);

    colX = attributes + 1;
    colY = 1;
    rowY = rowX;

    double** trainMatrix = readMatrix(fp, rowX, colX);

    double** matrixX = createMatrixX(trainMatrix, rowX, colX);
    double** matrixY = createMatrixY(trainMatrix, rowY);

    double** xTranspose = transposeMatrix(matrixX, rowX, colX);
    int colXT = rowX;
    int rowXT = colX;

    double** xTx = multiplyMatrix(xTranspose, matrixX, rowXT, colXT, rowX, colX);
    int rowxTx = rowXT;
    int colxTx = colX;

    double** xTxInv = invertMatrix(xTx, rowxTx);
    int rowxTxInv = rowxTx;
    int colxTxInv = colxTx;

    double** xTxInvMx = multiplyMatrix(xTxInv, xTranspose, rowxTxInv, colxTxInv, rowXT, colXT);
    int rowxTxInvMx = rowxTxInv;
    int colxTxInvMx = colXT;

    double** matrixW = multiplyMatrix(xTxInvMx, matrixY, rowxTxInvMx, colxTxInvMx, rowY, colY);
    int rowW = rowxTxInvMx;
    int colW = colY;

    int predictions;

    FILE* fp2 = fopen(argv[2], "r");

    fscanf(fp2, "%d\n", &predictions);

    double** matrixTest = readMatrixTest(fp2, predictions, attributes + 1);
    int rowTest = predictions;
    int colTest = attributes + 1;

    double** matrixResults = multiplyMatrix(matrixTest, matrixW, rowTest, colTest, rowW, colW);
    int rowResults = rowTest;
    int colResults = colW;
/*
    printf("\nTraining Matrix:\n");
    printMatrix(trainMatrix, rowX, colX);
    printf("\nMatrix X:\n");
    printMatrix(matrixX, rowX, colX);
    printf("\nMatrix Y:\n");
    printMatrix(matrixY, rowY, colY);
    printf("\nMatrix X^T:\n");
    printMatrix(xTranspose, rowXT, colXT);
    printf("\nMatrix X^T * X:\n");
    printMatrix(xTx, rowxTx, colxTx);
    printf("\nMatrix (X^T * X)^-1:\n");
    printMatrix(xTxInv, rowxTxInv, colxTxInv);
    printf("\nMatrix ((X^T * X)^-1)X^T:\n");
    printMatrix(xTxInvMx, rowxTxInvMx, colxTxInvMx);
    printf("\nMatrix W:\n");
    printMatrix(matrixW, rowW, colW);
    printf("\nMatrix Test:\n");
    printMatrix(matrixTest, rowTest, colTest);
    printf("\nMatrix Results:\n");*/
    printMatrix(matrixResults, rowResults, colResults);

    freeMatrix(trainMatrix, rowX);
    freeMatrix(matrixX, rowX);
    freeMatrix(matrixY, rowY);
    freeMatrix(xTranspose, colX);
    freeMatrix(xTx, colX);
    freeMatrix(xTxInv, rowxTxInv);
    freeMatrix(xTxInvMx, rowxTxInvMx);
    freeMatrix(matrixW, rowW);
    freeMatrix(matrixTest, rowTest);

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

double** readMatrix(FILE* fp, int row, int col){

    double** matrix = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * col);
    }

    for(int i = 0; i < row; i++){
        fscanf(fp, "%lf,", &matrix[i][0]);
        for(int j = 1; j < col - 1; j++){
            fscanf(fp, "%lf,", &matrix[i][j]);
        }
        fscanf(fp, "%lf\n", &matrix[i][col - 1]);
    }
    return matrix;
}

double** readMatrixTest(FILE* fp, int row, int col){
    double** matrix = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * col);
    }

    for(int i = 0; i < row; i++){
        for(int j = 1; j < col; j++){
            fscanf(fp, "%lf,", &matrix[i][j]);
        }
        matrix[i][0] = 1;
    }

    return matrix;
}

double** createMatrixX(double** matrix, int row, int col){

    double** matrixX = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        matrixX[i] = (double*)malloc(sizeof(double) * col);
    }

    for(int i = 0; i < row; i++){
        matrixX[i][0] = 1;
        for(int j = 1; j < col; j++){
            matrixX[i][j] = matrix[i][j];
        }
    }
    return matrixX;
}

double** createMatrixY(double** matrix, int row){

    double** matrixY = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        matrixY[i] = (double*)malloc(sizeof(double) * 1);
    }

    for(int i = 0; i < row; i++){
        matrixY[i][0] = matrix[i][0];
    }
    return matrixY;
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
        result[i] = (double*)malloc(sizeof(double)*col2);
    }

    for(int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            result[i][j] = 0;
        }
    }

    double sum = 0;

    for(int i = 0; i < row1; i++){
        for(int j = 0; j < col2; j++){
            for(int k = 0; k < row2; k++){
                sum = sum + m1[i][k] * m2[k][j];
            }
            result[i][j] = sum;
            sum = 0;
        }
    }
    return result;
}

double** invertMatrix(double** matrix, int row){

    double** copy = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        copy[i] = (double*)malloc(sizeof(double)*row);
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < row; j++){
            copy[i][j] = matrix[i][j];
        }
    }

    double** result = (double**)malloc(sizeof(double*)*row);

    for(int i = 0; i < row; i++) {
        result[i] = (double*)malloc(sizeof(double)*row);
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < row; j++){
            if(i == j){
                result[i][j] = 1;
            } else {
                result[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < row; i++){
        if(copy[i][i] != 1){
            if(copy[i][i] != 0){
                double divisor = copy[i][i];
                for(int j = 0; j < row; j++){
                    copy[i][j] = copy[i][j] / divisor;
                    result[i][j] = result[i][j] / divisor;
                }
            } else {
                double coef = 0;
                int nonZRow = 0;
                for(int j = 0; j < row; j++){
                    if(copy[j][i] != 0){
                        nonZRow = j;
                        coef = 1 / copy[j][i];
                        break;
                    }
                }
                for(int j = 0; j < row; j++){
                    copy[i][j] = copy[i][j] + copy[nonZRow][j] * coef;
                    result[i][j] = result[i][j] + result[nonZRow][j] * coef;
                }
            }
        }
        for(int j = i + 1; j < row; j++){
            if(i != j) {
                double coef = -1 * copy[j][i];
                for(int k = 0; k < row; k++) {
                    copy[j][k] = copy[j][k] + coef * copy[i][k];
                    result[j][k] = result[j][k] + coef * result[i][k];
                }
            }
        }
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < i; j++){
            double coef = -1 * copy[j][i];
            for(int k = 0; k < row; k++){
                copy[j][k] = copy[j][k] + coef * copy[i][k];
                result[j][k] = result[j][k] + coef * result[i][k];
            }
        }
    }

    freeMatrix(copy, row);
    return result;
}