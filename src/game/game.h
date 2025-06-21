#ifndef GAME_H
#define GAME_H

#include "sudoku.h"

typedef struct {
    char v;
    int row;
    int col;
} Move;

typedef struct {
    int points;
    int lifes;
    int size;
    int to_remove;
    int left;
    Board b;
} Game;

Game new_game(int size, int to_remove);
void destroy_game(Game* g);
Move move(char val, int row, int col);
int play(Move m, Game* g);

#endif
