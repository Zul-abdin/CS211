#include "stdlib.h"
#include "stdio.h"

// Creates space for a 9x9 sudoku board
unsigned int** CreateBoard() {
    unsigned int** board = (unsigned int**)malloc(sizeof(unsigned int*) * 9);
    unsigned int i;
    for (i = 0; i < 9; i++)
        board[i] = (unsigned int*)malloc(sizeof(unsigned int) * 9);
    
    return board;
}

// Free the space for 9x9 sudoku board
void FreeBoard(unsigned int** board) {
    unsigned int i;
    for (i = 0; i < 9; i++)
        free(board[i]);
    free(board);
}

// Output the sudoku board to stdout.
void PrintBoard(unsigned int** board) {
    unsigned int i, j;
    for (i = 0; i < 9; i ++) {
        for (j = 0; j <9; j++) printf("%u\t", board[i][j]);
        printf("\n");
    }
}

// Read a file and populate sudoku board.
void ReadBoard(unsigned int** board, char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) exit(0);

    unsigned int i, j;
    char c;
    // Initially read as character
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fscanf(fp, " %c", &c);
            // If c == '-', store 0. Otherwise, store the number.
            board[i][j] = (c == 45) ? 0 : c - '0';
        }
    }
}

// returns 1 if the board is sane and okay.
// returns 0 if the board is insane, not okay, and doesn't make sence.
int CheckSane(unsigned int** board) {
    unsigned int r, c, r2, c2, boxr, boxc;
    for (r = 0; r < 9; r++) {
        for (c = 0; c < 9; c++) {
            if (board[r][c]) {
                // Check conflict horizontally
                for (c2 = 0; c2 < 9; c2++) {
                    if (c != c2 && board[r][c] == board[r][c2]) return 0;
                }
                // Check conflict vertically
                for (r2 = 0; r2 < 9; r2++) {
                    if (r != r2 && board[r][c] == board[r2][c]) return 0;
                }
                // Check conflict in the box
                boxr = r / 3 * 3; // Top of the box
                boxc = c / 3 * 3; // Leftside of the box
                for (r2 = boxr; r2 < boxr + 3; r2++) {
                    for (c2 = boxc; c2 < boxc + 3; c2++) {
                        if (r != r2 && c != c2 && board[r][c] == board[r2][c2]) return 0;
                    }
                }
            }
        }
    }
    
    return 1;
}

// returns 1 if board is solved.
// returns 0 if board is not solved.
int SolveBoard(unsigned int** board) {
    // If the board we have is insane, then return 0.
    if (!CheckSane(board)) return 0;
    
    // find an empty slot
    unsigned int newr, newc, foundEmpty = 0;
    for (newr = 0; newr < 9; newr++) {
        for (newc = 0; newc < 9; newc++) {
            if (!board[newr][newc]) {
                foundEmpty = 1;
                break;
            }
        }
        if (foundEmpty) break;
    }
    
    // If there is no empty slot, we've solved the Sudoku!
    if (!foundEmpty) return 1;
    
    // If there is still empty slots, then let's fill it with a random value!
    unsigned int i;
    for (i = 1; i <= 9; i++) {
        board[newr][newc] = i;
        if (SolveBoard(board)) return 1;
    }
    
    // Backtrack. Reset the slot to empty.
    board[newr][newc] = 0;
    return 0;
}

int main(int argc, char** argv) {
    
    unsigned int** board = CreateBoard();
    ReadBoard(board, argv[1]);
    
    if (!SolveBoard(board)) printf("no-solution\n");
    else PrintBoard(board);
    
    FreeBoard(board);
    return 0;
}




























