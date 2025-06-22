#ifndef SUDOKU_H
#define SUDOKU_H

#define EMPTY '0'

typedef char** Board;

Board create_board(int size);
void free_board(Board board, int size);
int is_valid(Board board, int size, int row, int col, char num);
Board generate_sudoku(Board board, int size, int to_remove);

#endif
