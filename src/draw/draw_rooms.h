#ifndef DRAW_ROOMS_H
#define DRAW_ROOMS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "../states/states.h"
#include "../game/game.h"

void init_font();
void destroy_font();
void init_color();

void draw_menu_room(int mouseX, int mouseY);
void draw_config_room(int mouseX, int mouseY);
void draw_difficulty_room(int mouseX, int mouseY, GameState *gameState);
void draw_game_room(int mouseX, int mouseY, GameState *gameState, Game *game, GameState *op_gameState, Game *op_game);
void draw_ip_room(int mouseX, int mouseY, OnlineState *online_state);
void draw_waiting_room(int mouseX, int mouseY, OnlineState * online_state);
void draw_victory_room(int mouseX, int mouseY, OnlineState *online_state);
void draw_lose_room(int mouseX, int mouseY, OnlineState *online_state);
void draw_history_room(int mouseX, int mouseY);

#endif
