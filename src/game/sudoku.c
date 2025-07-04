#include "sudoku.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char** Board;

// create a board
Board create_board(int size) {
    Board board = (Board)malloc(size * sizeof(char*));
    int i;
    for (i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
        memset(board[i], EMPTY, size);
    }
    return board;
}

// 
void free_board(Board board, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

int is_valid(Board board, int size, int row, int col, char num) {
    int i;
    for (i = 0; i < size; i++) {
        if (board[row][i] == num && i != col) return 0;
        if (board[i][col] == num && i != row) return 0;

        int boxSize = (int)sqrt(size);
        int boxRow = boxSize * (row / boxSize) + i / boxSize;
        int boxCol = boxSize * (col / boxSize) + i % boxSize;
        if (boxRow == row && boxCol == col) continue;
        if (board[boxRow][boxCol] == num) return 0;
    }
    return 1;
}

int fill_board(Board board, int size, int row, int col) {
    if (row == size) return 1;

    int nextRow = (col == size - 1) ? row + 1 : row;
    int nextCol = (col + 1) % size;

    char nums[size];
    int i;
    for (i = 0; i < size; i++) nums[i] = '1' + i;

    for (i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }

    for (i = 0; i < size; i++) {
        if (is_valid(board, size, row, col, nums[i])) {
            board[row][col] = nums[i];
            if (fill_board(board, size, nextRow, nextCol)) return 1;
            board[row][col] = EMPTY;
        }
    }
    return 0;
}

void remove_cells(Board board, int size, int holes) {
    while (holes > 0) {
        int r = rand() % size;
        int c = rand() % size;
        if (board[r][c] != EMPTY) {
            board[r][c] = EMPTY;
            holes--;
        }
    }
}

Board generate_sudoku(Board board, int size, int to_remove) {
    srand(time(NULL));
    fill_board(board, size, 0, 0);
    int i,j;
    Board gab = create_board(size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            gab[i][j] = board[i][j];
        }
    }
    remove_cells(board, size, to_remove);
    return gab;
}
