#ifndef AUDIO_H
#define AUDIO_H

#include <allegro5/allegro_audio.h>

extern ALLEGRO_SAMPLE *som_acerto;
extern ALLEGRO_SAMPLE *som_erro;

void init_audio();
void destroy_audio();

#endif
