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

Game new_game(int size, int to_remove) {
    Game g;
    g.p1_points = 0;
    g.p2_points = 0;
    g.p1_lifes = 5;
    g.p2_lifes = 5;
    g.size = size;
    g.to_remove = to_remove;

    g.b = create_board(size);
    generate_sudoku(g.b, size, to_remove);
    g.left = to_remove;

    return g;
}

void destroy_game(Game* g) {
    free_board(g->b, g->size);
}

typedef struct {
    char v;
    int row;
    int col;
    unsigned short player_1;
} Move;

Move move(char val, int row, int col, unsigned short player_1) {
    Move m;
    m.v = val;
    m.row = row;
    m.col = col;
    m.player_1 = player_1;
    return m;
}

int play(Move m, Game* g) {
    if (m.row < 0 || m.row >= g->size || m.col < 0 || m.col >= g->size)
        return 0;

    if (g->b[m.row][m.col] != EMPTY)
        return 0;

    if (is_valid(g->b, g->size, m.row, m.col, m.v)) {
        g->b[m.row][m.col] = m.v;
        if (m.player_1)
            g->p1_points++;
        else
            g->p2_points++;
        g->left--;
        return 1;
    }

    if (m.player_1)
        g->p1_lifes--;
    else
        g->p2_lifes--;

    return 0;
}
