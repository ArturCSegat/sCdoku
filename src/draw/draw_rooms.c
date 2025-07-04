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

// Constants for UI elements
const int LEFT_MARGIN = 30;        // Button margin for UI elements
const int BUTTON_WIDTH = VIRTUAL_W/3; // Width of buttons
const int BUTTON_HEIGHT = 50;      // Height of buttons
const int BUTTON_PADDING = 20;     // Space between buttons
const int TEXT_PADDING_X = 10;     // Padding for text

// Color variables
ALLEGRO_COLOR backgroundColor;     // Background color
ALLEGRO_COLOR buttonColor;        // Normal button color
ALLEGRO_COLOR buttonHoverColor;   // Button color when hovered
ALLEGRO_COLOR textButtonColor;    // Text color
ALLEGRO_COLOR titleColor;         // Title color
ALLEGRO_COLOR selectedBoxColor;    // Box color when selected
ALLEGRO_COLOR sameLineColumnColor; // Box color when selected
ALLEGRO_COLOR lineGradeColor; // Box color when selected
ALLEGRO_COLOR grayColor;   // Box color when selected
ALLEGRO_COLOR numberColor;   // Box color when selected
ALLEGRO_COLOR errorBoxColor;   // Box color when selected
ALLEGRO_COLOR errorTextColor;   // Box color when selected

// Font resources
ALLEGRO_FONT *font;               // Regular font
ALLEGRO_FONT *fontTitle;         // Title font
ALLEGRO_FONT *fontNumber;         // Title font
ALLEGRO_FONT *fontSmall;         // Title font

// Initialize font resources
void init_font(){
    font = al_load_ttf_font("assets/font/bdb.TTF", 40, 0);     // Load regular font at 44pt
    fontTitle = al_load_ttf_font("assets/font/font.TTF", 70, 0); // Load title font at 50pt
    fontNumber = al_load_ttf_font("assets/font/bdb.TTF", 30, 0); // Load number font at 30pt
    fontSmall = al_load_ttf_font("assets/font/bdb.TTF", 20, 0); // Load number font at 30pt
}

void destroy_font(){
    al_destroy_font(font);
    al_destroy_font(fontTitle);
    al_destroy_font(fontNumber);
    al_destroy_font(fontSmall);
}

void init_color(){
    backgroundColor = al_map_rgb(10, 10, 15);
    grayColor = al_map_rgb(44, 47, 66);
    buttonColor = al_map_rgb(50, 122, 232);
    buttonHoverColor = al_map_rgb(23, 96, 207);
    textButtonColor = al_map_rgb(255, 255, 255);
    titleColor = al_map_rgb(176, 180, 208);
    selectedBoxColor = al_map_rgb(37, 47, 109);
    sameLineColumnColor = al_map_rgb(31, 33, 46);
    lineGradeColor = al_map_rgb(2, 5, 1);
    numberColor = al_map_rgb(176, 180, 208);
    errorBoxColor = al_map_rgb(91,55,66);
    errorTextColor = al_map_rgb(186,83,92);
}

// Draw the main menu room
void draw_menu_room(int mouseX, int mouseY) {
    init_color();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_clear_to_color(backgroundColor);

    // Calculate button layout
    const int NUM_BUTTONS = 4;
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
    int buttonX = LEFT_MARGIN;

    al_draw_filled_rectangle(0, 0,
        (VIRTUAL_W/3), VIRTUAL_H,
        grayColor);

    al_draw_text(fontTitle, titleColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Menu options
    const char* menuOptions[] = { "SOLO", "MULTIPLAYER", "CONFIGURACOES", "SAIR" };

    int i;
    // Draw buttons
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonY = firstButtonY + i * (BUTTON_HEIGHT + BUTTON_PADDING);

        // Check if mouse is over this button
        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + BUTTON_WIDTH &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_HEIGHT;

        // Set appropriate button color
        ALLEGRO_COLOR color = isHovered ?
            buttonHoverColor :
            buttonColor;

        // Draw button background
        al_draw_filled_rectangle(buttonX, buttonY,
                                BUTTON_WIDTH - LEFT_MARGIN,
                                buttonY + BUTTON_HEIGHT,
                                color);

        // Draw centered text
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textButtonColor,
                    buttonX + TEXT_PADDING_X, textY,
                    ALLEGRO_ALIGN_LEFT, menuOptions[i]);
    }
}

// Draw the configuration room
void draw_config_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_draw_filled_rectangle(0, 0,
        (VIRTUAL_W/3), VIRTUAL_H,
        grayColor);

    al_draw_text(fontTitle, titleColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Calculate button layout
    const int NUM_BUTTONS = 2;
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
    int buttonX = LEFT_MARGIN;

    // Configuration options
    const char* menuOptions[] = { "FULL SCREEN", "BACK" };

    int i;
    // Draw buttons
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonY = firstButtonY + i * (BUTTON_HEIGHT + BUTTON_PADDING);

        // Check if mouse is over this button
        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + BUTTON_WIDTH &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_HEIGHT;

        // Set appropriate button color
        ALLEGRO_COLOR color = isHovered ?
            buttonHoverColor :
            buttonColor;

        // Draw button background
        al_draw_filled_rectangle(buttonX, buttonY,
            BUTTON_WIDTH - LEFT_MARGIN,
            buttonY + BUTTON_HEIGHT,
            color);

        // Draw centered text
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textButtonColor,
                    buttonX + TEXT_PADDING_X, textY,
                    ALLEGRO_ALIGN_LEFT, menuOptions[i]);
    }
}

// Draw the difficulty selection room
void draw_difficulty_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw logo and title
    al_draw_text(fontTitle, titleColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");
    al_draw_text(fontTitle, titleColor, VIRTUAL_W / 2, 200, ALLEGRO_ALIGN_CENTER, "Game Difficulty");

    // Define difficulty buttons
    const int BUTTON_SIZE = 150;      // Size of each difficulty button
    const int BUTTON_SPACING = 30;    // Space between buttons
    const int NUM_BUTTONS = 3;        // Number of difficulty levels
    const char* difficulties[] = { "EASY", "MEDIUM", "HARD" };

    // Calculate layout
    int totalWidth = NUM_BUTTONS * BUTTON_SIZE + (NUM_BUTTONS - 1) * BUTTON_SPACING;
    int startX = (VIRTUAL_W - totalWidth) / 2;
    int buttonY = VIRTUAL_H / 2;

    int i;
    // Draw difficulty buttons
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonX = startX + i * (BUTTON_SIZE + BUTTON_SPACING);

        // Check if mouse is over this button
        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + BUTTON_SIZE &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_SIZE;

        // Set appropriate button color
        ALLEGRO_COLOR color = isHovered ? buttonHoverColor : buttonColor;

        // Draw button background
        al_draw_filled_rectangle(buttonX, buttonY,
                                 buttonX + BUTTON_SIZE,
                                 buttonY + BUTTON_SIZE,
                                 color);

        // Draw centered text
        int textWidth = al_get_text_width(font, difficulties[i]);
        int textHeight = al_get_font_line_height(font);
        int textX = buttonX + (BUTTON_SIZE - textWidth) / 2;
        int textY = buttonY + (BUTTON_SIZE - textHeight) / 2;

        al_draw_text(font, textButtonColor, textX, textY, ALLEGRO_ALIGN_LEFT, difficulties[i]);
    }

    // Draw back button below difficulty buttons
    const int BACK_BUTTON_WIDTH = 150;
    const int BACK_BUTTON_HEIGHT = 40;
    const int BACK_BUTTON_SPACING_Y = 40;

    int backButtonX = (VIRTUAL_W - BACK_BUTTON_WIDTH) / 2;
    int backButtonY = buttonY + BUTTON_SIZE + BACK_BUTTON_SPACING_Y;

    // Check if mouse is over back button
    bool isBackHovered =
        mouseX >= backButtonX &&
        mouseX <= backButtonX + BACK_BUTTON_WIDTH &&
        mouseY >= backButtonY &&
        mouseY <= backButtonY + BACK_BUTTON_HEIGHT;

    // Set appropriate button color
    ALLEGRO_COLOR backColor = isBackHovered ? buttonHoverColor : buttonColor;

    // Draw back button
    al_draw_filled_rectangle(backButtonX, backButtonY,
                             backButtonX + BACK_BUTTON_WIDTH,
                             backButtonY + BACK_BUTTON_HEIGHT,
                             backColor);

    // Draw back button text
    int backTextHeight = al_get_font_line_height(font);
    int backTextY = backButtonY + (BACK_BUTTON_HEIGHT - backTextHeight) / 2;

    al_draw_text(font, textButtonColor, backButtonX + TEXT_PADDING_X, backTextY,
                 ALLEGRO_ALIGN_LEFT, "BACK");
}

void draw_game_room(int mouseX, int mouseY, GameState *gameState, Game *game) {
    init_color();
    al_clear_to_color(backgroundColor);

    const int GRID_SIZE = 9;
    const int CELL_SIZE = 50;
    const int gridWidth = GRID_SIZE * CELL_SIZE;
    const int gridHeight = GRID_SIZE * CELL_SIZE;

    const int PANEL_PADDING = 10;
    const int HEADER_HEIGHT = 40;
    const int PANEL_WIDTH = gridWidth + PANEL_PADDING * 2;
    const int PANEL_HEIGHT = gridHeight + HEADER_HEIGHT + PANEL_PADDING * 2;
    const int TEXT_OFFSET_Y = 6;

    const int panelX = (VIRTUAL_W - PANEL_WIDTH) / 2;
    const int panelY = (VIRTUAL_H - PANEL_HEIGHT) / 2;

    al_draw_filled_rounded_rectangle(
        panelX, panelY,
        panelX + PANEL_WIDTH, panelY + PANEL_HEIGHT,
        20, 20,
        grayColor
    );

    char statusText[64];
    const char *difficulty_str;
    switch (game->difficulty) {
        case DIFFICULTY_EASY:
            difficulty_str = "FACIL";
            break;
        case DIFFICULTY_MEDIUM:
            difficulty_str = "MEDIO";
            break;
        case DIFFICULTY_HARD:
            difficulty_str = "DIFICIL";
            break;
        default:
            difficulty_str = "NONE";
            break;
    }

    char diffText[32];
    sprintf(diffText, "DIFICULDADE: %s", difficulty_str);
    al_draw_text(fontSmall, titleColor,
                panelX + PANEL_PADDING,
                panelY + PANEL_PADDING + TEXT_OFFSET_Y,
                ALLEGRO_ALIGN_LEFT, diffText);

    char errosText[16];
    sprintf(errosText, "ERROS: %d/3", (3 - game->lifes));
    al_draw_text(fontSmall, titleColor,
                panelX + PANEL_WIDTH / 2,
                panelY + PANEL_PADDING + TEXT_OFFSET_Y,
                ALLEGRO_ALIGN_CENTER, errosText);

    // Tempo decorrido (timer) alinhado à direita da borda
    double now = al_get_time();
    int elapsedSeconds = (int)(now - gameState->startTime);
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;

    char timerText[32];
    sprintf(timerText, "TEMPO: %02d:%02d", minutes, seconds);

    al_draw_text(fontSmall, titleColor,
                 panelX + PANEL_WIDTH - PANEL_PADDING,
                 panelY + PANEL_PADDING + TEXT_OFFSET_Y,
                 ALLEGRO_ALIGN_RIGHT, timerText);

    const int startX = panelX + PANEL_PADDING;
    const int startY = panelY + PANEL_PADDING + HEADER_HEIGHT;

    int row, col;
    int selectedRow = gameState->selectedRow;
    int selectedCol = gameState->selectedCol;
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;

            al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, grayColor);
        }
    }

    if (selectedRow >= 0 && selectedCol >= 0) {
        int boxStartRow = (selectedRow / 3) * 3;
        int boxStartCol = (selectedCol / 3) * 3;

        for (row = 0; row < GRID_SIZE; row++) {
            for (col = 0; col < GRID_SIZE; col++) {
                bool sameRow = (row == selectedRow);
                bool sameCol = (col == selectedCol);
                bool sameBox = (row >= boxStartRow && row < boxStartRow + 3 &&
                                col >= boxStartCol && col < boxStartCol + 3);

                if (sameRow || sameCol || sameBox) {
                    int x = startX + col * CELL_SIZE;
                    int y = startY + row * CELL_SIZE;

                    al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, sameLineColumnColor);
                }
            }
        }

        if (selectedRow >= 0 && selectedCol >= 0) {
            char selectedVal = game->b[selectedRow][selectedCol];
    
            if (selectedVal != EMPTY) {
                for (row = 0; row < GRID_SIZE; row++) {
                    for (col = 0; col < GRID_SIZE; col++) {
                        if (game->b[row][col] == selectedVal) {
                            int x = startX + col * CELL_SIZE;
                            int y = startY + row * CELL_SIZE;
                            al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, selectedBoxColor);
                        }
                    }
                }
            }
    
            int selX = startX + selectedCol * CELL_SIZE;
            int selY = startY + selectedRow * CELL_SIZE;
            al_draw_filled_rectangle(selX, selY, selX + CELL_SIZE, selY + CELL_SIZE, selectedBoxColor);
        }
    }
    
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;
            if (gameState->errors[row][col]) {
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, errorBoxColor);
            }
        }
    }

    int i;
    // Desenhar linhas da grade
    for (i = 0; i <= GRID_SIZE; i++) {
        // Linhas horizontais
        al_draw_line(startX, startY + i * CELL_SIZE,
                     startX + gridWidth, startY + i * CELL_SIZE,
                     lineGradeColor, (i % 3 == 0) ? 3 : 1);

        // Linhas verticais
        al_draw_line(startX + i * CELL_SIZE, startY,
                     startX + i * CELL_SIZE, startY + gridHeight,
                     lineGradeColor, (i % 3 == 0) ? 3 : 1);
    }

    // printa os numeros
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            char value = game->b[row][col];
            if (value == EMPTY)
                value = gameState->attempts[row][col];
            if (value != EMPTY) {
                int x = startX + (col * CELL_SIZE) + CELL_SIZE / 2;
                int y = startY + (row * CELL_SIZE) + CELL_SIZE / 4;

                char text[2] = { value, '\0' };
                ALLEGRO_COLOR color = numberColor;
                if (gameState->errors[row][col] && selectedRow == row && selectedCol == col) {
                    color = errorTextColor;
                }
                al_draw_text(fontNumber, color, x, y, ALLEGRO_ALIGN_CENTER, text);
            }
        }
    }
}

void draw_waiting_room(int mouseX, int mouseY, OnlineState*online_state) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_draw_filled_rectangle(0, 0,
        (VIRTUAL_W/3), VIRTUAL_H,
        grayColor);

    al_draw_text(fontTitle, titleColor, 30, 100, ALLEGRO_ALIGN_LEFT, "scdoku");
    if (online_state->is_admin) {
        al_draw_text(fontSmall, titleColor, 600, 150, ALLEGRO_ALIGN_CENTER, "Aguardando conecao de outor jogador");
    } else {
        al_draw_text(fontSmall, titleColor, 600, 150, ALLEGRO_ALIGN_CENTER, "Voce está conectado, aguarde o começo do jogo");
    }
}

void draw_ip_room(int mouseX, int mouseY, OnlineState *online_state) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_draw_text(fontTitle, titleColor, VIRTUAL_W / 2, 100, ALLEGRO_ALIGN_CENTER, "DIGITE O IP");

    const int BOX_WIDTH = 400;
    const int BOX_HEIGHT = 60;
    int boxX = (VIRTUAL_W - BOX_WIDTH) / 2;
    int boxY = VIRTUAL_H / 2 - BOX_HEIGHT / 2;

    al_draw_filled_rectangle(boxX, boxY, boxX + BOX_WIDTH, boxY + BOX_HEIGHT, grayColor);
    al_draw_rectangle(boxX, boxY, boxX + BOX_WIDTH, boxY + BOX_HEIGHT,
                      online_state->ip_invalid ? errorBoxColor : buttonColor, 2);

    al_draw_text(font, numberColor, boxX + TEXT_PADDING_X, boxY + 10, ALLEGRO_ALIGN_LEFT, online_state->ip);

    if (online_state->ip_invalid) {
        al_draw_text(font, errorTextColor, VIRTUAL_W / 2, boxY + BOX_HEIGHT + 10,
                     ALLEGRO_ALIGN_CENTER, "IP invalido, tente novamente!");
    }

    const int BUTTON_HEIGHT = 50;
    const int BUTTON_PADDING = 20;
    
    int BUTTON_WIDTH = (BOX_WIDTH - BUTTON_PADDING) / 2;
    int buttonY = boxY + BOX_HEIGHT + 80;
    int voltarX = boxX;
    int avancarX = voltarX + BUTTON_WIDTH + BUTTON_PADDING;

    bool isBackHovered = mouseX >= voltarX && mouseX <= voltarX + BUTTON_WIDTH &&
                         mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT;
    ALLEGRO_COLOR backColor = isBackHovered ? buttonHoverColor : buttonColor;

    al_draw_filled_rectangle(voltarX, buttonY,
                             voltarX + BUTTON_WIDTH, buttonY + BUTTON_HEIGHT,
                             backColor);

    al_draw_text(font, textButtonColor, voltarX + TEXT_PADDING_X,
                 buttonY + (BUTTON_HEIGHT - al_get_font_line_height(font)) / 2,
                 ALLEGRO_ALIGN_LEFT, "VOLTAR");

    bool isNextHovered = mouseX >= avancarX && mouseX <= avancarX + BUTTON_WIDTH &&
                         mouseY >= buttonY && mouseY <= buttonY + BUTTON_HEIGHT;
    ALLEGRO_COLOR nextColor = isNextHovered ? buttonHoverColor : buttonColor;

    al_draw_filled_rectangle(avancarX, buttonY,
                             avancarX + BUTTON_WIDTH, buttonY + BUTTON_HEIGHT,
                             nextColor);

    al_draw_text(font, textButtonColor, avancarX + TEXT_PADDING_X,
                 buttonY + (BUTTON_HEIGHT - al_get_font_line_height(font)) / 2,
                 ALLEGRO_ALIGN_LEFT, "AVANCAR");
}
