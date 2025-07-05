#include "sudoku.h"
#include "game.h"
#include <stdio.h>
#include <stdbool.h>

#include "../states/states.h"

// create new game
Game new_game(int size, int to_remove) {
    Game game;
    game.points = 0;
    game.lifes = 3;
    game.size = size;
    game.to_remove = to_remove;
    game.difficulty = DIFFICULTY_NONE;
    game.b = create_board(size);
    game.gab = generate_sudoku(game.b, size, to_remove);
    game.left = to_remove;

    return game;
}

// free game memory
void destroy_game(Game* game) {
    free_board(game->b, game->size);
}

// make move function
Move move(char val, int row, int col) {
    Move m;
    m.v = val;
    m.row = row;
    m.col = col;
    return m;
}


int play(Move m, Game* game) {
    if (m.row < 0 || m.row >= game->size || m.col < 0 || m.col >= game->size)
        return 0;

    // Impede sobrescrever os números fixos do jogo (ou seja, não removidos)
    if (game->gab[m.row][m.col] == game->b[m.row][m.col])
        return 0;

    // Se estiver apagando (val == EMPTY), apenas atualiza a grade, sem penalizar
    if (m.v == EMPTY) {
        game->b[m.row][m.col] = EMPTY;
        return 0;
    }

    // Se estiver correto
    if (m.v == game->gab[m.row][m.col]) {
        // Se antes estava errado (não EMPTY e diferente do correto), decrementa `left`
        if (game->b[m.row][m.col] != game->gab[m.row][m.col])
            game->left--;

        game->b[m.row][m.col] = m.v;
        return 1;
    }

    // Se estiver errado, ainda salva na grade e subtrai vida
    game->b[m.row][m.col] = m.v;
    game->lifes--;
    return 0;
}
