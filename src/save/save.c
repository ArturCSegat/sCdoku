#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "../save/save.h"

void salvar_historico(int tempo_em_segundos, const char* dificuldade, int erros, bool venceu) {
    FILE* arquivo = fopen(HISTORICO_PATH, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de historico!\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm* data_hora = localtime(&agora);

    fprintf(
        arquivo,
        "%04d-%02d-%02d %02d:%02d:%02d;%d;%s;%d;%s\n",
        data_hora->tm_year + 1900, data_hora->tm_mon + 1, data_hora->tm_mday,
        data_hora->tm_hour, data_hora->tm_min, data_hora->tm_sec,
        tempo_em_segundos,
        dificuldade,
        erros,
        venceu ? "Vitoria" : "Derrota"
    );

    fclose(arquivo);
}

int ler_historico(Partida partidas[], int max_partidas) {
    FILE* arquivo = fopen(HISTORICO_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de historico para leitura!\n");
        return 0;
    }

    int count = 0;
    while (count < max_partidas && !feof(arquivo)) {
        Partida p;
        char resultado[10];

        if (fscanf(arquivo, "%19[^;];%d;%19[^;];%d;%9s\n",
                   p.data_hora, &p.tempo, p.dificuldade, &p.erros, resultado) == 5) {
            p.venceu = (strcmp(resultado, "Vitoria") == 0);
            partidas[count++] = p;
        }else {
            break;
        }
    }

    fclose(arquivo);
    return count;
}
