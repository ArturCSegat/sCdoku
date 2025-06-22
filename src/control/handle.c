#include <stdbool.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "../config.h"
#include "../states/states.h"
#include "../game/game.h"
#include "../draw/draw_rooms.h"

#ifdef _WIN32
#include "../online/online_win.h"
#else
#include "../online/online_unix.h"
#endif

int handle_menu_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room){
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_WIDTH = 300;
        const int BUTTON_HEIGHT = 40;
        const int BUTTON_PADDING = 10;
        const int LEFT_MARGIN = 30;
        const int NUM_BUTTONS = 3;

        int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
        int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
        int buttonX = LEFT_MARGIN;


        // START
        int startButtonY = firstButtonY;
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= startButtonY && logicalMouseY <= startButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_DIFFICULTY;
        }

        // CONFIG
        int configButtonY = firstButtonY + 1 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= configButtonY && logicalMouseY <= configButtonY + BUTTON_HEIGHT) {
            *current_room = ROOM_CONFIG;
        }

        // EXIT
        int exitButtonY = firstButtonY + 2 * (BUTTON_HEIGHT + BUTTON_PADDING);
        if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_WIDTH &&
            logicalMouseY >= exitButtonY && logicalMouseY <= exitButtonY + BUTTON_HEIGHT) {
            return 0; // Sinaliza que deve sair
        }
    }

    return 1; // Continua rodando
}

int handle_config_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room, bool *is_fullscreen){
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_WIDTH = 300;
        const int BUTTON_HEIGHT = 40;
        const int BUTTON_PADDING = 10;
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

Difficulty handle_difficulty_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameRoom *current_room) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int BUTTON_SIZE = 150;
        const int BUTTON_SPACING = 30;
        const int NUM_BUTTONS = 3;

        int totalWidth = NUM_BUTTONS * BUTTON_SIZE + (NUM_BUTTONS - 1) * BUTTON_SPACING;
        int startX = (VIRTUAL_W - totalWidth) / 2;
        int buttonY = VIRTUAL_H / 2;

        int i;
        // Verifica cliques nas dificuldades
        for (i = 0; i < NUM_BUTTONS; i++) {
            int buttonX = startX + i * (BUTTON_SIZE + BUTTON_SPACING);

            if (logicalMouseX >= buttonX && logicalMouseX <= buttonX + BUTTON_SIZE &&
                logicalMouseY >= buttonY && logicalMouseY <= buttonY + BUTTON_SIZE) {
                *current_room = ROOM_GAME;
                return (Difficulty)i;
            }
        }

        // Botão BACK
        const int BACK_BUTTON_WIDTH = 150;
        const int BACK_BUTTON_HEIGHT = 40;
        const int BACK_BUTTON_SPACING_Y = 40;

        int backButtonX = (VIRTUAL_W - BACK_BUTTON_WIDTH) / 2;
        int backButtonY = buttonY + BUTTON_SIZE + BACK_BUTTON_SPACING_Y;

        if (logicalMouseX >= backButtonX && logicalMouseX <= backButtonX + BACK_BUTTON_WIDTH &&
            logicalMouseY >= backButtonY && logicalMouseY <= backButtonY + BACK_BUTTON_HEIGHT) {
            *current_room = ROOM_MENU;
            return DIFFICULTY_NONE;
        }
    }

    return DIFFICULTY_NONE;
}

void handle_game_events(ALLEGRO_EVENT ev, int logicalMouseX, int logicalMouseY, GameState *gameState, Game *game, ON_SOCK opp) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
        const int GRID_SIZE = 9;
        const int CELL_SIZE = 50;
        int gridWidth = GRID_SIZE * CELL_SIZE;
        int gridHeight = GRID_SIZE * CELL_SIZE;

        int startX = (VIRTUAL_W - gridWidth) / 2;
        int startY = (VIRTUAL_H - gridHeight) / 2;

        if (logicalMouseX >= startX && logicalMouseX < startX + gridWidth &&
            logicalMouseY >= startY && logicalMouseY < startY + gridHeight) {

            int col = (logicalMouseX - startX) / CELL_SIZE;
            int row = (logicalMouseY - startY) / CELL_SIZE;

            gameState->selectedRow = row;
            gameState->selectedCol = col;
        }
    }

    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
        int row = gameState->selectedRow;
        int col = gameState->selectedCol;

        if (row >= 0 && col >= 0) {
            char key = ev.keyboard.unichar;

            if (key >= '1' && key <= '9') {
                Move m = move(key, row, col);
                int result = play(m, game);

                // Só atualiza visualmente se a jogada for aceita (mesmo correta ou incorreta)
                if (result == 1 || game->b[row][col] == key) {
                    gameState->attempts[row][col] = key;
                    gameState->errors[row][col] = (key != game->gab[row][col]);
                }

                if (result == 1) {
                    online_send(opp, "val", 3);
                }
            }

            if (key == ALLEGRO_KEY_BACKSPACE || key == '0') {
                // Evita apagar números fixos
                if (game->gab[row][col] != game->b[row][col]) {
                    Move m = move(EMPTY, row, col);
                    play(m, game);
                    gameState->errors[row][col] = false;
                    gameState->attempts[row][col] = EMPTY;
                }
            }
        }
    }
}
