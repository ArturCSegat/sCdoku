#include <stdio.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "audio.h"

ALLEGRO_SAMPLE *som_acerto = NULL;
ALLEGRO_SAMPLE *som_erro = NULL;
ALLEGRO_SAMPLE *som_scc = NULL;

void init_audio() {
    if (!al_install_audio()) {
        fprintf(stderr, "Erro ao instalar audio.\n");
        exit(1);
    }
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Erro ao iniciar codecs de audio.\n");
        exit(1);
    }
    if (!al_reserve_samples(16)) {
        fprintf(stderr, "Erro ao reservar samples.\n");
        exit(1);
    }

    som_acerto = al_load_sample("./assets/audio/acerto.ogg");
    som_erro = al_load_sample("./assets/audio/erro.ogg");
    som_scc = al_load_sample("./assets/audio/scc.ogg");

    if (!som_acerto || !som_erro || !som_scc) {
        fprintf(stderr, "Erro ao carregar arquivos de audio.\n");
        exit(1);
    }
}

void destroy_audio() {
    if (som_acerto) al_destroy_sample(som_acerto);
    if (som_erro) al_destroy_sample(som_erro);
    if (som_scc) al_destroy_sample(som_scc);
    al_uninstall_audio();
}
