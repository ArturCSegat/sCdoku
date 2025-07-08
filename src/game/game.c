#include "sudoku.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
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

void to_char(char *dst, int len, Game *g, GameState *gs) {
    if (len < _max_game_str_len) {
        fprintf(stderr, "to_char do Game recebeu um buffer pequeno demais");
        exit(69);
    }

    int row,col;
    int i = 0;
    for (row = 0; row < g->size; row++) {
        for (col = 0; col < g->size; col++) {
            dst[row*g->size + col] = g->b[row][col];
            i++;
        }
    }
    dst[i++] = '0'+gs->selectedRow;
    dst[i++] = ':';
    dst[i++] = '0'+gs->selectedCol;
    dst[i++] = '0'+g->difficulty;
    dst[i++] = '0'+g->lifes;
    
    int ei=0, ej=0;
    for(ei = 0;ei<9;ei++) {
        for(ej = 0;ej<9;ej++) {
            dst[i++] = gs->errors[ei][ej] ? '1':'0';
        }
    }

    for(ei = 0;ei<9;ei++) {
        for(ej = 0;ej<9;ej++) {
            dst[i++] = gs->attempts[ei][ej];
        }
    }

    dst[i++] = 0;
}

void from_char(char *from, int len, Game*g, GameState *gs) {
    if (len < _max_game_str_len) {
        fprintf(stderr, "from_char do Game recebeu uma msg de tamanho inesperado: crashando");
        exit(69);
    }

    int c = 0;
    int i, j, k = 0;
    for (i = 0; i < g->size; i++) {
        for (j = 0; j < g->size; j++) {
            if (from[i*g->size + j] == EMPTY) c++;
            g->b[i][j] = from[i*g->size + j];
            k++;
        }
    }
    int row = from[k++]-'0';
    k++;
    int col = from[k++]-'0';
    gs->selectedRow = row;
    gs->selectedCol = col;
    g->difficulty = from[k++]-'0';
    g->lifes = from[k++]-'0';

    int ei=0, ej=0;
    for(ei = 0;ei<9;ei++) {
        for(ej = 0;ej<9;ej++) {
            gs->errors[ei][ej] = from[k++]=='1';
        }
    }

    for(ei = 0;ei<9;ei++) {
        for(ej = 0;ej<9;ej++) {
            gs->attempts[ei][ej] = from[k++];
        }
    }
}
