all: BinaryTree SudokuSolver

BinaryTree: BinaryTree.c
	gcc -Wall -Werror -fsanitize=address BinaryTree.c -o BinaryTree

SudokuSolver: SudokuSolver.c
	gcc -Wall -Werror -fsanitize=address SudokuSolver.c -o SudokuSolver

clean:
	rm -rf BinaryTree SudokuSolver