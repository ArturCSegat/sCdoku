#ifndef GAME_H
#define GAME_H

#include "sudoku.h"

typedef struct {
    int p1_points;
    int p2_points;
    int p1_lifes;
    int p2_lifes;
    Board b;
    int left;
    int size;
    int to_remove;
} Game;

typedef struct {
    char v;
    int row;
    int col;
    unsigned short player_1;
} Move;

Game new_game(int size, int to_remove);
void destroy_game(Game* g);
Move move(char val, int row, int col, unsigned short player_1);
int play(Move m, Game* g);

#endif
