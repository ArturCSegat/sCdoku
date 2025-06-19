#ifndef SUDOKU_H
#define SUDOKU_H

#define EMPTY '.'

// Define o tipo Board como um ponteiro duplo (matriz alocada dinamicamente)
typedef char** Board;

Board create_board(int size);
void free_board(Board board, int size);

int is_valid(Board board, int size, int row, int col, char num);
int fill_board(Board board, int size, int row, int col);
void remove_cells(Board board, int size, int holes);
void generate_sudoku(Board board, int size, int to_remove);

#endif
