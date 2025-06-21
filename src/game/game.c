#include "sudoku.h"
#include "game.h"
#include <stdbool.h>

// create new game
Game new_game(int size, int to_remove) {
    Game g;
    g.points = 0;
    g.lifes = 5;
    g.size = size;
    g.to_remove = to_remove;

    g.b = create_board(size);
    generate_sudoku(g.b, size, to_remove);
    g.left = to_remove;

    return g;
}

// free game memory
void destroy_game(Game* g) {
    free_board(g->b, g->size);
}

// make move function
Move move(char val, int row, int col) {
    Move m;
    m.v = val;
    m.row = row;
    m.col = col;
    return m;
}

// do a move and verify if the player lose life or not
int play(Move m, Game* g) {
    if (m.row < 0 || m.row >= g->size || m.col < 0 || m.col >= g->size)
        return 0;

    if (g->b[m.row][m.col] != EMPTY)
        return 0;

    if (is_valid(g->b, g->size, m.row, m.col, m.v)) {
        g->b[m.row][m.col] = m.v;
        g->left--;
        return 1;
    }
    g->lifes--;
    return 0;
}
