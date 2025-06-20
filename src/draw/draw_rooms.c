#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "../config.h"
#include "../states/states.h"

// Constants for UI elements
const int BUTTON_WIDTH = 300;      // Width of buttons
const int BUTTON_HEIGHT = 40;      // Height of buttons
const int BUTTON_PADDING = 10;     // Space between buttons
const int TEXT_PADDING_X = 10;     // Padding for text
const int LEFT_MARGIN = 30;        // Left margin for UI elements

// Color variables
ALLEGRO_COLOR backgroundColor;     // Background color
ALLEGRO_COLOR buttonColor;        // Normal button color
ALLEGRO_COLOR buttonHoverColor;   // Button color when hovered
ALLEGRO_COLOR textColor;          // Text color

// Font resources
ALLEGRO_FONT *font;               // Regular font
ALLEGRO_FONT *fontTitle;         // Title font

// Initialize font resources
void init_font(){
    font = al_load_ttf_font("assets/font/font.TTF", 24, 0);     // Load regular font at 24pt
    fontTitle = al_load_ttf_font("assets/font/font.TTF", 50, 0); // Load title font at 50pt
}

void destroy_font(){
    al_destroy_font(font);
    al_destroy_font(fontTitle);
}

void init_color(){
    backgroundColor = al_map_rgb(102, 153, 255);
    buttonColor = al_map_rgb(0, 43, 128);
    buttonHoverColor = al_map_rgb(0, 63, 128);
    textColor = al_map_rgb(255, 255, 255);
}

// Draw the main menu room
void draw_menu_room(int mouseX, int mouseY) {
    init_color();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_clear_to_color(backgroundColor);
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Calculate button layout
    const int NUM_BUTTONS = 3;
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
    int buttonX = LEFT_MARGIN;

    // Menu options
    const char* menuOptions[] = { "START", "CONFIG", "EXIT" };

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
                                buttonX + BUTTON_WIDTH,
                                buttonY + BUTTON_HEIGHT,
                                color);

        // Draw centered text
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textColor,
                    buttonX + TEXT_PADDING_X, textY,
                    ALLEGRO_ALIGN_LEFT, menuOptions[i]);
    }
}

// Draw the configuration room
void draw_config_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

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
                                buttonX + BUTTON_WIDTH,
                                buttonY + BUTTON_HEIGHT,
                                color);

        // Draw centered text
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textColor,
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
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");
    al_draw_text(fontTitle, buttonColor, VIRTUAL_W / 2, 200, ALLEGRO_ALIGN_CENTER, "Game Difficulty");

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

        al_draw_text(font, textColor, textX, textY, ALLEGRO_ALIGN_LEFT, difficulties[i]);
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

    al_draw_text(font, textColor, backButtonX + TEXT_PADDING_X, backTextY,
                 ALLEGRO_ALIGN_LEFT, "BACK");
}

void draw_game_room(int mouseX, int mouseY, GameState *gameState) {
    init_color();
    al_clear_to_color(backgroundColor);

    const int GRID_SIZE = 9;
    const int CELL_SIZE = 50;
    int gridWidth = GRID_SIZE * CELL_SIZE;
    int gridHeight = GRID_SIZE * CELL_SIZE;

    int startX = (VIRTUAL_W - gridWidth) / 2;
    int startY = (VIRTUAL_H - gridHeight) / 2;

    //
    // Destacar celula selecionada
    //
    if (gameState->selectedRow >= 0 && gameState->selectedCol >= 0) {
        int x = startX + gameState->selectedCol * CELL_SIZE;
        int y = startY + gameState->selectedRow * CELL_SIZE;

        al_draw_filled_rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE, buttonColor);
    }

    //
    // Desenhar linhas da grade
    //
    int i;
    for (i = 0; i <= GRID_SIZE; i++) {
        // Linhas horizontais
        al_draw_line(startX, startY + i * CELL_SIZE,
                        startX + gridWidth, startY + i * CELL_SIZE,
                        textColor, (i % 3 == 0) ? 3 : 1);

        // Linhas verticais
        al_draw_line(startX + i * CELL_SIZE, startY,
                        startX + i * CELL_SIZE, startY + gridHeight,
                        textColor, (i % 3 == 0) ? 3 : 1);
    }

    // Aqui voce pode desenhar os numeros do tabuleiro se quiser
}
