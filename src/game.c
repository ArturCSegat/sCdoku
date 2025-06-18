#include "sudoku.c"

typedef struct {
    int p1_points;
    int p2_points;
    int p1_lifes;
    int p2_lifes;
    Board b;
    int left;
} Game;

Game new_game() {
    Game g;
    g.p1_points = 0;
    g.p2_points = 0;
    g.p1_lifes = 5;
    g.p2_lifes = 5;
    generate_sudoku(g.b);
    g.left = TO_REMOVE;
    return g;
}

void print_game(Game * g) {
    print_board(g->b);
    printf("p1 lifes: %d\tp2 lifes: %d\n", g->p1_lifes, g->p2_lifes);
    printf("p1 points: %d\tp2 points: %d\n", g->p1_points, g->p2_points);
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

int play(Move m, Game * g) {
    if (is_valid(g->b, m.row, m.col, m.v)) {
        g->b[m.row][m.col] = m.v;
        if (m.player_1) g->p1_points++;
        else g->p2_points++;
        g->left--;
        return 1;
    }
    if (m.player_1) g->p1_lifes--;
    else g->p2_lifes--;
    return 0;
}

