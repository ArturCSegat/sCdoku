#include <stdbool.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "../config.h"
#include "../states/states.h"
#include "../game/game.h"
#include "../draw/draw_rooms.h"
#include "./validation/ip_validation.h"
#include "../audio/audio.h"

#ifdef _WIN32
#include "../online/online_win.h"
#else
#include "../online/online_unix.h"
#endif

int handle_menu_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *gameState, GameRoom *current_room){
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_WIDTH = VIRTUAL_W/3;
        const int BUTTON_HEIGHT = 50;
        const int BUTTON_PADDING = 20;
        const int LEFT_MARGIN = 30;
        const int NUM_BUTTONS = 4;

        int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
        int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
        int buttonX = LEFT_MARGIN;


        // START
        int startButtonY = firstButtonY;
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= startButtonY && logicalMouseY <= startButtonY + BUTTON_HEIGHT) {
            gameState->isOnline = false;
            *current_room = ROOM_DIFFICULTY;
        }

        // MULTIPLAYER
        int multiplayerButtonY = firstButtonY + 1 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= multiplayerButtonY && logicalMouseY <= multiplayerButtonY + BUTTON_HEIGHT) {
            gameState->isOnline = true;
            *current_room = ROOM_IP;
        }

        // CONFIG
        int configButtonY = firstButtonY + 2 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= configButtonY && logicalMouseY <= configButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_CONFIG;
        }

        // EXIT
        int exitButtonY = firstButtonY + 3 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= exitButtonY && logicalMouseY <= exitButtonY + BUTTON_HEIGHT) {
            return 0; // Sinaliza que deve sair
        }
    }

    return 1; // Continua rodando
}

int handle_config_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, bool *is_fullscreen){
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_WIDTH = VIRTUAL_W/3;
        const int BUTTON_HEIGHT = 50;
        const int BUTTON_PADDING = 20;
        const int LEFT_MARGIN = 30;
        const int NUM_BUTTONS = 2;

        int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
        int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
        int buttonX = LEFT_MARGIN;

        // FULL SCREEN
        int fullScreenButtonY = firstButtonY;
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= fullScreenButtonY && logicalMouseY <= fullScreenButtonY + BUTTON_HEIGHT) {
            *is_fullscreen = !*is_fullscreen;
            al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, *is_fullscreen);
            destroy_font();
            init_font();

            if (!*is_fullscreen) {
                al_set_window_position(display, 0, 0);
            }
        }

        // BACK
        int backButtonY = firstButtonY + (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= backButtonY && logicalMouseY <= backButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_MENU;
        }
    }

    return 1; // Continua rodando
}

void handle_difficulty_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState* online_state, Game * game, GameState *gameState) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_SIZE = 150;
        const int BUTTON_SPACING = 30;
        const int NUM_BUTTONS = 3;

        int totalWidth = NUM_BUTTONS * BUTTON_SIZE + (NUM_BUTTONS - 1) * BUTTON_SPACING;
        int startX = (VIRTUAL_W - totalWidth) / 2;
        int buttonY = VIRTUAL_H / 2;

        int i;
        int to_remove;
        for (i = 0; i < NUM_BUTTONS; i++) {
            int buttonX = startX + i * (BUTTON_SIZE + BUTTON_SPACING);

            if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_SIZE &&
                logicalMouseY >= buttonY && logicalMouseY <= buttonY + BUTTON_SIZE) {
                switch(i){
                    case 0: to_remove = 45; break;
                    case 1: to_remove = 53; break;
                    case 2: to_remove = 57; break;
                }
                *current_room = ROOM_GAME;
                *game = new_game(SIZE, to_remove, 3);
                switch(i){
                    case 0: game->difficulty = DIFFICULTY_EASY; break;
                    case 1: game->difficulty = DIFFICULTY_MEDIUM; break;
                    case 2: game->difficulty = DIFFICULTY_HARD; break;
                }
                char msg[_max_game_str_len] = {0};
                to_char(msg, _max_game_str_len, game, gameState);
                int r = online_send(online_state->opponent, msg, _max_game_str_len);
                online_state->done = true;
                gameState->startTime = al_get_time();
            }
        }

        const int BACK_BUTTON_WIDTH = 150;
        const int BACK_BUTTON_HEIGHT = 40;
        const int BACK_BUTTON_SPACING_Y = 40;

        int backButtonX = (VIRTUAL_W - BACK_BUTTON_WIDTH) / 2;
        int backButtonY = buttonY + BUTTON_SIZE + BACK_BUTTON_SPACING_Y;

        if (logicalMouseX >= backButtonX && logicalMouseX <= backButtonX + BACK_BUTTON_WIDTH &&
            logicalMouseY >= backButtonY && logicalMouseY <= backButtonY + BACK_BUTTON_HEIGHT) {
            *current_room = ROOM_MENU;
        }
    }
}

void handle_game_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *gameState, Game *game, OnlineState *opp, GameRoom *current_room) {
    if(game->left == 0){
        //*current_room = ROOM_VICTORY;
    }
    if(game->lifes == 0){
        //*current_room = ROOM_LOSE;
    }
    
    opp->waiting = false;

    const int GRID_SIZE = 9;
    const int CELL_SIZE = 50;
    const int gridWidth = GRID_SIZE * CELL_SIZE;
    const int gridHeight = GRID_SIZE * CELL_SIZE;

    const int PANEL_PADDING = 10;
    const int HEADER_HEIGHT = 40;
    const int PANEL_WIDTH = gridWidth + PANEL_PADDING * 2;
    const int PANEL_HEIGHT = gridHeight + HEADER_HEIGHT + PANEL_PADDING * 2;

    int panelX, startX, startY;
    if (gameState->isOnline) {
        panelX = (VIRTUAL_W / 4) - (PANEL_WIDTH / 2);  // Painel do jogador local (esquerda)
    } else {
        panelX = (VIRTUAL_W - PANEL_WIDTH) / 2; // Centralizado
    }

    startX = panelX + PANEL_PADDING;
    startY = (VIRTUAL_H - PANEL_HEIGHT) / 2 + PANEL_PADDING + HEADER_HEIGHT;

    // Clique do mouse
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        if (logicalMouseX >= startX && logicalMouseX < startX + gridWidth &&
            logicalMouseY >= startY && logicalMouseY < startY + gridHeight) {

            int col = (logicalMouseX - startX) / CELL_SIZE;
            int row = (logicalMouseY - startY) / CELL_SIZE;

            gameState->selectedRow = row;
            gameState->selectedCol = col;
        }
        goto update;
    }

    // Teclas de navegação (setas)
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        int row = gameState->selectedRow;
        int col = gameState->selectedCol;

        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                if (row > 0) row--;
                break;
            case ALLEGRO_KEY_DOWN:
                if (row < 8) row++;
                break;
            case ALLEGRO_KEY_LEFT:
                if (col > 0) col--;
                break;
            case ALLEGRO_KEY_RIGHT:
                if (col < 8) col++;
                break;
        }

        gameState->selectedRow = row;
        gameState->selectedCol = col;
        goto update;
    }

    // Digitação de número ou backspace
    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
        int row = gameState->selectedRow;
        int col = gameState->selectedCol;

        if (row >= 0 && col >= 0) {
            char key = ev.keyboard.unichar;

            if (key >= '1' && key <= '9') {
                Move m = move(key, row, col);
                int result = play(m, game);

                // se jogada possivel
                if (result == 1 || game->b[row][col] == key) {
                    if(key == game->gab[row][col]){
                        al_play_sample(som_acerto, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }else{
                        al_play_sample(som_erro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                    gameState->attempts[row][col] = key;
                    gameState->errors[row][col] = (key != game->gab[row][col]);
                }
            }

            if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE || key == '0') {
                // Evita apagar número fixo
                if (game->gab[row][col] != game->b[row][col]) {
                    Move m = move(EMPTY, row, col);
                    play(m, game);
                    al_play_sample(som_erro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    gameState->errors[row][col] = false;
                    gameState->attempts[row][col] = EMPTY;
                }
            }
            goto update;
        }
    }

    return;
    update:
    if (gameState->isOnline) {
        char msg[_max_game_str_len] = {0};
        to_char(msg, _max_game_str_len, game, gameState);
        online_send(opp->opponent, msg, _max_game_str_len); // sinaliza jogada
    }

}


void handle_ip_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState *online_state, GameState *gameState) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        int mouseX = logicalMouseX;
        int mouseY = logicalMouseY;

        // Alinhamento igual ao da draw
        const int BOX_WIDTH = 400;
        const int BOX_HEIGHT = 60;
        int boxX = (VIRTUAL_W - BOX_WIDTH) / 2;

        const int BUTTON_PADDING = 20;
        int BUTTON_WIDTH = (BOX_WIDTH - BUTTON_PADDING) / 2;
        int BUTTON_HEIGHT = 50;
        int buttonY = (VIRTUAL_H / 2 - BOX_HEIGHT / 2) + BOX_HEIGHT + 80;

        int voltarX = boxX;
        int avancarX = voltarX + BUTTON_WIDTH + BUTTON_PADDING;

        // Botão VOLTAR
        if (mouseX >= voltarX && mouseX <= voltarX + BUTTON_WIDTH &&
            mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
            *current_room = ROOM_MENU;
            online_state->ip[0] = '\0';
            online_state->ip_invalid = false;
        }


        // Botão AVANÇAR
        if (mouseX >= avancarX && mouseX <= avancarX + BUTTON_WIDTH &&
            mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT) {
            goto ip_val;
        }
    }

    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
        char ch = ev.keyboard.unichar;

        // Inserção de número ou ponto
        if ((ch >= '0' && ch <= '9') || ch == '.') {
            size_t len = strlen(online_state->ip);
            if (len < 15) {
                online_state->ip[len] = ch;
                online_state->ip[len + 1] = '\0';
            }
        }

        // Backspace
        if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
            size_t len = strlen(online_state->ip);
            if (len > 0) {
                online_state->ip[len - 1] = '\0';
            }
        }

        // ENTER: valida IP
        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            goto ip_val;
        }
    }

    return;
    ip_val:
        if (is_valid_ip(online_state->ip)) {
            online_state->ip_invalid = false;
            online_state->ip_invalid = false;
            online_state->opponent = connect_to(online_state->ip);
            if (online_state->opponent == (-1)) {
                online_state->is_admin = true;
            }

            *current_room = ROOM_WAITING;
        } else {
            online_state->ip_invalid = true;
        }
}

void handle_waiting_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState *online_state, Game * game, GameState *gameState) {
    if (!online_state->waiting) {
        online_state->waiting = true;
        return;
    }

    if (online_state->opponent != -1) {
        if (!online_state->is_admin) {
            char b[_max_game_str_len]; // vai receber 2 tabs um sendo o gab
            online_recv(online_state->opponent, b, _max_game_str_len); // blocking read
            online_state->done = true;
            game->b = create_board(SIZE);
            game->gab = create_board(SIZE);
            game->size = SIZE;
            from_char(b, _max_game_str_len, game, gameState);

            *current_room = ROOM_GAME;
            return;
        }
    };

    online_state->opponent = connect_to(online_state->ip);
    if (online_state->opponent==(-1)) {
        online_state->opponent = get_oponnent();
        online_state->is_admin = true;
    }

    if(!online_state->is_admin){
        *current_room = ROOM_WAITING;
        return;
    }
    *current_room = ROOM_DIFFICULTY;
}


void handle_msg(char*msg, int msg_len, GameState * op_game_state, Game * op_game) {
    from_char(msg, msg_len, op_game, op_game_state);
}

