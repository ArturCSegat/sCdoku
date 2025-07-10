#ifndef HANDLE_H
#define HANDLE_H

#include "../states/states.h"
#include "../game/game.h"

#ifdef _WIN32
#include "../online/online_win.h"
#else
#include "../online/online_unix.h"
#endif

int handle_menu_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *gameState, GameRoom *current_room);
int handle_config_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, bool *is_fullscreen);
void handle_difficulty_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState * online_state, Game * game, GameState *gameState, GameState * op_game_state, Game * op_game);
int handle_game_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *game_state, Game *game, OnlineState *opp, GameRoom *current_room);
void handle_ip_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState *online_state, GameState *gameState);
void handle_waiting_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState *online_state, Game * game, GameState *gameState, GameState * op_game_state, Game * op_game);
void handle_victory_room_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY,Game * game, GameState *gameState, GameRoom *current_room);
void handle_lose_room_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY,Game * game, GameState *gameState, GameRoom *current_room);
void handle_history_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room);

void handle_msg(char*msg, int msg_len, GameState * op_game_state, Game * op_game);

#endif
