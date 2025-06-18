#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE (9)
#define TO_REMOVE (40)
#define EMPTY '.'

typedef char Board[SIZE][SIZE];

void print_board(Board board) {
    int i;
    for (i =0; i<SIZE; i++) {
        printf("|");
        int j;
        for (j =0; j<SIZE; j++) {
            printf(" %c |", board[i][j]);
        }
        printf("\n");
    }
}

int is_valid(Board board, int row, int col, char num) {
    int i;
    for (i = 0; i < SIZE; i++) {
        if (board[row][i] == num && i != col)return 0;
        if (board[i][col] == num && i != row)return 0;

        int boxRow = 3 * (row / 3) + i / 3;
        int boxCol = 3 * (col / 3) + i % 3;
        if (boxCol == col && boxRow == row) continue;
        if (board[boxRow][boxCol] == num)
            return 0;
    }
    return 1;
}

int fill_board(Board board, int row, int col) {
    if (row == SIZE) return 1;

    int nextRow = (col == SIZE - 1) ? row + 1 : row;
    int nextCol = (col + 1) % SIZE;

    char nums[SIZE];
    int i;
    for (i = 0; i < SIZE; i++)
        nums[i] = '1' + i;

    for (i = SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }

    for (i = 0; i < SIZE; i++) {
        if (is_valid(board, row, col, nums[i])) {
            board[row][col] = nums[i];
            if (fill_board(board, nextRow, nextCol))
                return 1;
            board[row][col] = EMPTY;
        }
    }
    return 0;
}

// Remove cells to create the puzzle
void remove_cells(Board board, int holes) {
    while (holes > 0) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (board[r][c] != EMPTY) {
            board[r][c] = EMPTY;
            holes--;
        }
    }
}

// Main function to initialize a new Sudoku board
void generate_sudoku(Board board) {
    memset(board, '.', SIZE*SIZE);
    srand(time(NULL));
    fill_board(board, 0, 0);
    remove_cells(board, 40);
}

