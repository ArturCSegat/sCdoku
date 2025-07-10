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
ALLEGRO_COLOR boxColor;            // cor da caixinha dos menus
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
    boxColor = al_map_rgb(18, 20, 27);
    buttonColor = al_map_rgb(36, 39, 59);
    buttonHoverColor = al_map_rgb(46, 49, 79);
    textButtonColor = al_map_rgb(102, 153, 255);
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
    int boxWidth = BUTTON_WIDTH + 2 * LEFT_MARGIN;
    int boxHeight = 450;
    int boxX = (VIRTUAL_W - boxWidth) / 2;
    int boxY = (VIRTUAL_H - boxHeight) / 2;
    int buttonX = boxX + LEFT_MARGIN;
    int firstButtonY = boxY + (boxHeight - totalMenuHeight) / 2 + 40;

    al_draw_filled_rounded_rectangle(
        boxX, boxY,
        boxX + boxWidth,
        boxY + boxHeight,
        20, 20,
        boxColor);

    al_draw_text(fontTitle, titleColor,
             boxX + boxWidth / 2,
             boxY + 30,
             ALLEGRO_ALIGN_CENTER,
             "sCdoku");

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
        al_draw_filled_rounded_rectangle(
                            buttonX, buttonY,
                            buttonX + BUTTON_WIDTH,
                            buttonY + BUTTON_HEIGHT,
                            10, 10, // raio de curvatura
                            color
        );

        // Draw centered text
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textButtonColor,
                buttonX + BUTTON_WIDTH / 2, textY,
                ALLEGRO_ALIGN_CENTER, menuOptions[i]);
    }
}

void draw_config_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    const float sidebarWidth = VIRTUAL_W / 3;
    al_draw_filled_rectangle(0, 0, sidebarWidth, VIRTUAL_H, grayColor);
    al_draw_text(fontTitle, titleColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Ajuste largura segura para os botões padrão
    const int SAFE_BUTTON_WIDTH = sidebarWidth - 2 * LEFT_MARGIN;

    // Botões padrão
    const int NUM_BUTTONS = 2;
    const char* menuOptions[] = { "FULL SCREEN", "BACK" };
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2 - 80;
    int buttonX = LEFT_MARGIN;

    int i;
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonY = firstButtonY + i * (BUTTON_HEIGHT + BUTTON_PADDING);

        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + SAFE_BUTTON_WIDTH &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_HEIGHT;

        ALLEGRO_COLOR color = isHovered ? buttonHoverColor : buttonColor;

        al_draw_filled_rounded_rectangle(
            buttonX, buttonY,
            buttonX + SAFE_BUTTON_WIDTH,
            buttonY + BUTTON_HEIGHT,
            12, 12, color
        );

        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textButtonColor,
                     buttonX + SAFE_BUTTON_WIDTH / 2,
                     textY,
                     ALLEGRO_ALIGN_CENTER, menuOptions[i]);
    }

    // Controle volume layout
    int controlCenterX = buttonX + SAFE_BUTTON_WIDTH / 2;
    int circleRadius = 16;
    int spacingX = 45;
    int spacingY = 100;

    // Posicionamento Y para blocos de volume
    int soundLabelY = firstButtonY + NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) + 60;
    int soundControlY = soundLabelY + 30;

    int musicLabelY = soundControlY + spacingY;
    int musicControlY = musicLabelY + 30;

    // Posicionamento X dos botões
    int soundMinusX = controlCenterX - spacingX;
    int soundPlusX  = controlCenterX + spacingX;
    int musicMinusX = controlCenterX - spacingX;
    int musicPlusX  = controlCenterX + spacingX;

    // Detectar hover do mouse para os botões
    bool soundMinusHover = (mouseX >= soundMinusX - circleRadius && mouseX <= soundMinusX + circleRadius &&
                            mouseY >= soundControlY - circleRadius && mouseY <= soundControlY + circleRadius);
    bool soundPlusHover  = (mouseX >= soundPlusX - circleRadius && mouseX <= soundPlusX + circleRadius &&
                            mouseY >= soundControlY - circleRadius && mouseY <= soundControlY + circleRadius);

    bool musicMinusHover = (mouseX >= musicMinusX - circleRadius && mouseX <= musicMinusX + circleRadius &&
                            mouseY >= musicControlY - circleRadius && mouseY <= musicControlY + circleRadius);
    bool musicPlusHover  = (mouseX >= musicPlusX - circleRadius && mouseX <= musicPlusX + circleRadius &&
                            mouseY >= musicControlY - circleRadius && mouseY <= musicControlY + circleRadius);

    // Ajuste para posicionar labels um pouco acima dos controles com padding maior
    int labelOffsetY = 35; // Padding maior entre label e controle

    // Desenhar labels centralizadas acima dos controles
    al_draw_text(font, al_map_rgb(255, 255, 255), controlCenterX, soundLabelY - labelOffsetY, ALLEGRO_ALIGN_CENTER, "SOUND VOLUME");
    al_draw_text(font, al_map_rgb(255, 255, 255), controlCenterX, musicLabelY - labelOffsetY, ALLEGRO_ALIGN_CENTER, "MUSIC VOLUME");

    // Desenhar botões SOUND VOLUME
    al_draw_filled_circle(soundMinusX, soundControlY, circleRadius, soundMinusHover ? buttonHoverColor : buttonColor);
    al_draw_text(font, textButtonColor, soundMinusX, soundControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, "-");

    char soundStr[10];
    snprintf(soundStr, sizeof(soundStr), "%.1f", SOUND_VOLUME);
    al_draw_text(font, al_map_rgb(255, 255, 255), controlCenterX, soundControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, soundStr);

    al_draw_filled_circle(soundPlusX, soundControlY, circleRadius, soundPlusHover ? buttonHoverColor : buttonColor);
    al_draw_text(font, textButtonColor, soundPlusX, soundControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, "+");

    // Desenhar botões MUSIC VOLUME
    al_draw_filled_circle(musicMinusX, musicControlY, circleRadius, musicMinusHover ? buttonHoverColor : buttonColor);
    al_draw_text(font, textButtonColor, musicMinusX, musicControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, "-");

    char musicStr[10];
    snprintf(musicStr, sizeof(musicStr), "%.1f", MUSIC_VOLUME);
    al_draw_text(font, al_map_rgb(255, 255, 255), controlCenterX, musicControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, musicStr);

    al_draw_filled_circle(musicPlusX, musicControlY, circleRadius, musicPlusHover ? buttonHoverColor : buttonColor);
    al_draw_text(font, textButtonColor, musicPlusX, musicControlY - al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, "+");
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

void draw_single_board(int startX, int startY, GameState *gameState, Game *game, bool hide) {
    const int GRID_SIZE = 9;
    const int CELL_SIZE = 50;
    const int gridWidth = GRID_SIZE * CELL_SIZE;
    const int gridHeight = GRID_SIZE * CELL_SIZE;

    int row, col;

    // Draw base grid background
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;
            al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, grayColor);
        }
    }

    // Highlight selection row, column, and box
    int selectedRow = gameState->selectedRow;
    int selectedCol = gameState->selectedCol;

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

        char selectedVal = game->b[selectedRow][selectedCol];
        if (selectedVal != EMPTY) {
            for (row = 0; row < GRID_SIZE; row++) {
                for (col = 0; col < GRID_SIZE; col++) {
                    if (game->b[row][col] == selectedVal && !hide) {
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

    // Draw error boxes
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (gameState->errors[row][col]) {
                int x = startX + col * CELL_SIZE;
                int y = startY + row * CELL_SIZE;
                al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, errorBoxColor);
            }
        }
    }

    // Draw grid lines
    int i;
    for (i = 0; i <= GRID_SIZE; i++) {
        al_draw_line(startX, startY + i * CELL_SIZE, startX + gridWidth, startY + i * CELL_SIZE,
                     lineGradeColor, (i % 3 == 0) ? 3 : 1);
        al_draw_line(startX + i * CELL_SIZE, startY, startX + i * CELL_SIZE, startY + gridHeight,
                     lineGradeColor, (i % 3 == 0) ? 3 : 1);
    }

    // Draw numbers (or 'X' if hide is true)
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            char value = game->b[row][col];
            if (value == EMPTY)
                value = gameState->attempts[row][col];
            if (value != EMPTY) {
                int x = startX + (col * CELL_SIZE) + CELL_SIZE / 2;
                int y = startY + (row * CELL_SIZE) + CELL_SIZE / 4;

                char text[2] = { hide ? 'X' : value, '\0' };
                ALLEGRO_COLOR color = numberColor;

                if (gameState->errors[row][col] &&
                    gameState->selectedRow == row && gameState->selectedCol == col) {
                    color = errorTextColor;
                }

                al_draw_text(fontNumber, color, x, y, ALLEGRO_ALIGN_CENTER, text);
            }
        }
    }
}

void draw_game_room(int mouseX, int mouseY, GameState *gameState, Game *game, GameState *op_gameState, Game *op_game) {
    op_gameState->startTime = gameState->startTime;
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

    int panelX1, panelX2;
    int panelY = (VIRTUAL_H - PANEL_HEIGHT) / 2;

    if (gameState->isOnline) {
        panelX1 = (VIRTUAL_W / 4) - (PANEL_WIDTH / 2);
        panelX2 = (3 * VIRTUAL_W / 4) - (PANEL_WIDTH / 2);
    } else {
        panelX1 = (VIRTUAL_W - PANEL_WIDTH) / 2;
        panelX2 = -1;
    }

    // --- Panel 1 ---
    al_draw_filled_rounded_rectangle(panelX1, panelY,
                                     panelX1 + PANEL_WIDTH, panelY + PANEL_HEIGHT,
                                     20, 20, grayColor);

    char errosText1[16];
    sprintf(errosText1, "ERROS: %d/3", (3 - game->lifes));
    al_draw_text(fontSmall, titleColor, panelX1 + PANEL_PADDING,
                 panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_LEFT, errosText1);

    al_draw_text(fontSmall, titleColor, panelX1 + PANEL_WIDTH / 2,
                 panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_CENTER, "SEU  TABULEIRO");

    double now = al_get_time();
    int elapsedSeconds1 = (int)(now - gameState->startTime);
    int minutes1 = elapsedSeconds1 / 60;
    int seconds1 = elapsedSeconds1 % 60;
    char timerText1[32];
    sprintf(timerText1, "TEMPO: %02d:%02d", minutes1, seconds1);
    al_draw_text(fontSmall, titleColor, panelX1 + PANEL_WIDTH - PANEL_PADDING,
                 panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_RIGHT, timerText1);

    int startX1 = panelX1 + PANEL_PADDING;
    int startY1 = panelY + PANEL_PADDING + HEADER_HEIGHT;
    draw_single_board(startX1, startY1, gameState, game, false);

    // --- Panel 2 ---
    if (gameState->isOnline) {
        al_draw_filled_rounded_rectangle(panelX2, panelY,
                                         panelX2 + PANEL_WIDTH, panelY + PANEL_HEIGHT,
                                         20, 20, grayColor);

        char errosText2[16];
        sprintf(errosText2, "ERROS: %d/3", (3 - op_game->lifes));
        al_draw_text(fontSmall, titleColor, panelX2 + PANEL_PADDING,
                     panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_LEFT, errosText2);
    
        al_draw_text(fontSmall, titleColor, panelX2 + PANEL_WIDTH / 2,
                     panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_CENTER, "OPONENTE");

        int elapsedSeconds2 = (int)(now - op_gameState->startTime);
        int minutes2 = elapsedSeconds2 / 60;
        int seconds2 = elapsedSeconds2 % 60;
        char timerText2[32];
        sprintf(timerText2, "TEMPO: %02d:%02d", minutes2, seconds2);
        al_draw_text(fontSmall, titleColor, panelX2 + PANEL_WIDTH - PANEL_PADDING,
                     panelY + PANEL_PADDING + TEXT_OFFSET_Y, ALLEGRO_ALIGN_RIGHT, timerText2);

        int startX2 = panelX2 + PANEL_PADDING;
        int startY2 = panelY + PANEL_PADDING + HEADER_HEIGHT;
        draw_single_board(startX2, startY2, op_gameState, op_game, true);
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

void draw_victory_room(int mouseX, int mouseY, OnlineState *online_state) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_draw_text(fontTitle, titleColor, VIRTUAL_W / 2, 100, ALLEGRO_ALIGN_CENTER, "VITORIA");
}

void draw_lose_room(int mouseX, int mouseY, OnlineState *online_state) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_draw_text(fontTitle, titleColor, VIRTUAL_W / 2, 100, ALLEGRO_ALIGN_CENTER, "DERROTA");
}
