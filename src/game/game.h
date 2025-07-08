#ifndef GAME_H
#define GAME_H

#include "sudoku.h"
#include "../states/states.h"
// SIZE*SIZE(tab) + 3(selected coord) + 1(diff) + 1(lifes) + 81(errors) + 81(attempts) + 1(null)
#define _max_game_str_len 249

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
    Difficulty difficulty;
    Board b;
    Board gab;
} Game;

Game new_game(int size, int to_remove, int lifes);
void destroy_game(Game* g);
Move move(char val, int row, int col);
int play(Move m, Game* g);
void to_char(char * dst, int len,  Game *g, GameState *gs);
void from_char(char *from, int len, Game*g, GameState *gs);

#endif
