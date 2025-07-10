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
#include <string.h>
#include <math.h>

#include "../config.h"
#include "../states/states.h"
#include "../game/game.h"
#include "../draw/draw_rooms.h"
#include "./validation/ip_validation.h"
#include "../audio/audio.h"
#include "../save/save.h"

#ifdef _WIN32
#include "../online/online_win.h"
#else
#include "../online/online_unix.h"
#endif

int handle_menu_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *gameState, GameRoom *current_room) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_WIDTH = VIRTUAL_W/3;
        const int BUTTON_HEIGHT = 50;
        const int BUTTON_PADDING = 20;
        const int LEFT_MARGIN = 30;
        const int NUM_BUTTONS = 5;

        int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
        int boxWidth = BUTTON_WIDTH + 2 * LEFT_MARGIN;
        int boxHeight = 550;
        int boxX = (VIRTUAL_W - boxWidth) / 2;
        int boxY = (VIRTUAL_H - boxHeight) / 2;
        int buttonX = boxX + LEFT_MARGIN;
        int firstButtonY = boxY + (boxHeight - totalMenuHeight) / 2 + 20;

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

        // HISTORICO
        int historyButtonY = firstButtonY + 2 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= historyButtonY && logicalMouseY <= historyButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_HISTORY;
        }

        // CONFIG
        int configButtonY = firstButtonY + 3 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= configButtonY && logicalMouseY <= configButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_CONFIG;
        }

        // EXIT
        int exitButtonY = firstButtonY + 4 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= exitButtonY && logicalMouseY <= exitButtonY + BUTTON_HEIGHT) {
            return 0; // Sair do jogo
        }
    }

    return 1; // Continua no menu
}

void handle_config_events(ALLEGRO_EVENT ev, int mouseX, int mouseY, GameRoom *current_room) {
    if (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || ev.mouse.button != 1)
        return;

    const int LEFT_MARGIN = 30;
    const int BUTTON_HEIGHT = 50;
    const int BUTTON_PADDING = 20;
    const float sidebarWidth = VIRTUAL_W / 3;
    const int SAFE_BUTTON_WIDTH = sidebarWidth - 2 * LEFT_MARGIN;
    const int NUM_BUTTONS = 2;
    const int BUTTON_Y_START = (VIRTUAL_H - (NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING)) / 2 - 80;
    const int buttonX = LEFT_MARGIN;

    int fullScreenY = BUTTON_Y_START;
    bool clickedFullscreen = mouseX >= buttonX && mouseX <= buttonX + SAFE_BUTTON_WIDTH &&
                             mouseY >= fullScreenY && mouseY <= fullScreenY + BUTTON_HEIGHT;
    int backY = fullScreenY + BUTTON_HEIGHT + BUTTON_PADDING;
    bool clickedBack = mouseX >= buttonX && mouseX <= buttonX + SAFE_BUTTON_WIDTH &&
                       mouseY >= backY && mouseY <= backY + BUTTON_HEIGHT;

    if (clickedFullscreen) {
        al_toggle_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, true);
        return;
    }

    if (clickedBack) {
        *current_room = ROOM_MENU;
        return;
    }

    int controlCenterX = buttonX + SAFE_BUTTON_WIDTH / 2;
    int spacingX = 45;
    int spacingY = 100;
    int circleRadius = 16;

    int soundControlY = BUTTON_Y_START + NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) + 60 + 30;
    int musicControlY = soundControlY + spacingY;

    int soundMinusX = controlCenterX - spacingX;
    int soundPlusX = controlCenterX + spacingX;

    if (mouseX >= soundMinusX - circleRadius && mouseX <= soundMinusX + circleRadius &&
        mouseY >= soundControlY - circleRadius && mouseY <= soundControlY + circleRadius) {
        SOUND_VOLUME = fmaxf(0.0f, SOUND_VOLUME - 0.1f);
        al_play_sample(som_scc, SOUND_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        return;
    }

    if (mouseX >= soundPlusX - circleRadius && mouseX <= soundPlusX + circleRadius &&
        mouseY >= soundControlY - circleRadius && mouseY <= soundControlY + circleRadius) {
        SOUND_VOLUME = fminf(1.0f, SOUND_VOLUME + 0.1f);
        al_play_sample(som_scc, SOUND_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        return;
    }

    int musicMinusX = controlCenterX - spacingX;
    int musicPlusX = controlCenterX + spacingX;

    if (mouseX >= musicMinusX - circleRadius && mouseX <= musicMinusX + circleRadius &&
        mouseY >= musicControlY - circleRadius && mouseY <= musicControlY + circleRadius) {
        MUSIC_VOLUME = fmaxf(0.0f, MUSIC_VOLUME - 0.1f);
        al_play_sample(som_scc, MUSIC_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        return;
    }

    if (mouseX >= musicPlusX - circleRadius && mouseX <= musicPlusX + circleRadius &&
        mouseY >= musicControlY - circleRadius && mouseY <= musicControlY + circleRadius) {
        MUSIC_VOLUME = fminf(1.0f, MUSIC_VOLUME + 0.1f);
        al_play_sample(som_scc, MUSIC_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        return;
    }
}


void handle_difficulty_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState* online_state, Game * game, GameState *gameState, GameState * op_game_state, Game * op_game) {
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
                gameState->startTime = al_get_time();
                switch(i){
                    case 0: game->difficulty = DIFFICULTY_EASY; break;
                    case 1: game->difficulty = DIFFICULTY_MEDIUM; break;
                    case 2: game->difficulty = DIFFICULTY_HARD; break;
                }
                char msg[_max_game_str_len] = {0};
                to_char(msg, _max_game_str_len, game, gameState);
                int r = online_send(online_state->opponent, msg, _max_game_str_len);
                online_state->done = true;
                from_char(msg, _max_game_str_len, op_game, op_game_state);
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

void handle_waiting_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, OnlineState *online_state, Game * game, GameState *gameState, GameState * op_game_state, Game * op_game) {
    if (!online_state->waiting) {
        online_state->waiting = true;
        return;
    }

    if (online_state->opponent != -1) {
        if (!online_state->is_admin) {
            char b[_max_game_str_len];
            online_recv(online_state->opponent, b, _max_game_str_len); 
            online_state->done = true;
            *game = new_game(SIZE, 81, 3);
            from_char(b, _max_game_str_len, game, gameState);
            from_char(b, _max_game_str_len, op_game, op_game_state);
            gameState->startTime = al_get_time();

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

void handle_victory_room_events(ALLEGRO_EVENT ev, int mouseX, int mouseY,Game * game, GameState *gameState, GameRoom *current_room) {
    const int buttonWidth = 300;
    const int buttonHeight = 60;
    const int padding = 40;

    int buttonX = (VIRTUAL_W - buttonWidth) / 2;
    int boxHeight = buttonHeight + 2 * padding + 80;
    int boxY = (VIRTUAL_H - boxHeight) / 2;
    int buttonY = boxY + boxHeight - padding - buttonHeight;

    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        bool clicked = mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
                       mouseY >= buttonY && mouseY <= buttonY + buttonHeight;

        if (clicked) {
            char dificuldade[20];
            if (game->difficulty == 1) {
                strcpy(dificuldade, "Facil");
            } else if (game->difficulty == 2) {
                strcpy(dificuldade, "Medio");
            } else {
                strcpy(dificuldade, "Dificil");
            }
            salvar_historico(gameState->startTime, dificuldade, (3-game->lifes), true);
            clean_game(game, gameState);
            *current_room = ROOM_MENU;
        }
    }
}

void handle_lose_room_events(ALLEGRO_EVENT ev, int mouseX, int mouseY,Game * game, GameState *gameState, GameRoom *current_room) {
    const int buttonWidth = 300;
    const int buttonHeight = 60;
    const int padding = 40;

    int buttonX = (VIRTUAL_W - buttonWidth) / 2;
    int boxHeight = buttonHeight + 2 * padding + 80;
    int boxY = (VIRTUAL_H - boxHeight) / 2;
    int buttonY = boxY + boxHeight - padding - buttonHeight;

    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        bool clicked = mouseX >= buttonX && mouseX <= buttonX + buttonWidth &&
                       mouseY >= buttonY && mouseY <= buttonY + buttonHeight;

        if (clicked) {
            char dificuldade[20];
            if (game->difficulty == 1) {
                strcpy(dificuldade, "Facil");
            } else if (game->difficulty == 2) {
                strcpy(dificuldade, "Medio");
            } else {
                strcpy(dificuldade, "Dificil");
            }
            salvar_historico(gameState->startTime, dificuldade, (3-game->lifes), false);
            clean_game(game, gameState);
            *current_room = ROOM_MENU;
        }
    }
}

void clean_game(Game * game, GameState *gameState) {
    *game = new_game(SIZE, 81, 3);
    memset(gameState->attempts, EMPTY, 81);
    memset(gameState->errors, false, 81);
    gameState->isOnline = false;
    gameState->selectedRow = -1;
    gameState->selectedCol = -1;
    gameState->startTime = 0;
}

void handle_history_events(ALLEGRO_EVENT ev, int mouseX, int mouseY, GameRoom *current_room) {
    const int btn_w = 180;
    const int btn_h = 50;
    const int btn_x = VIRTUAL_W / 2 - btn_w / 2;
    const int boxHeight = 500;
    const int boxY = (VIRTUAL_H - boxHeight) / 2;
    const int btn_y = boxY + boxHeight - 70;

    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        bool isHovered = mouseX >= btn_x && mouseX <= btn_x + btn_w &&
                         mouseY >= btn_y && mouseY <= btn_y + btn_h;

        if (isHovered) {
            *current_room = ROOM_MENU;
        }
    }
}

void handle_msg(char*msg, int msg_len, GameState * op_game_state, Game * op_game) {
    from_char(msg, msg_len, op_game, op_game_state);
}

