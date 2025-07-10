#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>

#define MAX_PARTIDAS 1000

typedef struct {
    char data_hora[20];
    int tempo;
    char dificuldade[20];
    int erros;
    bool venceu;
} Partida;

#define HISTORICO_PATH "./assets/save/historico.txt"

void salvar_historico(int tempo_em_segundos, const char* dificuldade, int erros, bool venceu);
int ler_historico(Partida partidas[], int max_partidas);

#endif
