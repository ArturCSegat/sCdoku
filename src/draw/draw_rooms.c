#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "../config.h"

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 40;
const int BUTTON_PADDING = 10;
const int TEXT_PADDING_X = 10;
const int LEFT_MARGIN = 30;

ALLEGRO_COLOR backgroundColor;
ALLEGRO_COLOR buttonColor;
ALLEGRO_COLOR buttonHoverColor;
ALLEGRO_COLOR textColor;

ALLEGRO_FONT *font;
ALLEGRO_FONT *fontTitle;

void init_font(){
    font = al_load_ttf_font("assets/font/font.TTF", 24, 0);
    fontTitle = al_load_ttf_font("assets/font/font.TTF", 50, 0);
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

void draw_menu_room(int mouseX, int mouseY) {
    init_color();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_clear_to_color(backgroundColor);
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    const int NUM_BUTTONS = 3;
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
    int buttonX = LEFT_MARGIN;

    const char* menuOptions[] = { "START", "CONFIG", "EXIT" };
    int i;
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonY = firstButtonY + i * (BUTTON_HEIGHT + BUTTON_PADDING);

        // Detect hover using logical mouse coords
        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + BUTTON_WIDTH &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_HEIGHT;

        // Set button color based on hover state
        ALLEGRO_COLOR color = isHovered ?
            buttonHoverColor :
            buttonColor;

        // Draw the button rectangle
       al_draw_filled_rectangle(buttonX, buttonY,
                                buttonX + BUTTON_WIDTH,
                                buttonY + BUTTON_HEIGHT,
                                color);

        // Draw button text vertically centered
        int textHeight = al_get_font_line_height(font);
        int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

        al_draw_text(font, textColor,
                    buttonX + TEXT_PADDING_X, textY,
                    ALLEGRO_ALIGN_LEFT, menuOptions[i]);
    }
}

void draw_config_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    const int NUM_BUTTONS = 2;
    int totalMenuHeight = NUM_BUTTONS * (BUTTON_HEIGHT + BUTTON_PADDING) - BUTTON_PADDING;
    int firstButtonY = (VIRTUAL_H - totalMenuHeight) / 2;
    int buttonX = LEFT_MARGIN;

    const char* menuOptions[] = { "FULL SCREEN", "BACK" };
    int i;
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonY = firstButtonY + i * (BUTTON_HEIGHT + BUTTON_PADDING);

       // Detect hover using logical mouse coords
       bool isHovered =
           mouseX >= buttonX &&
           mouseX <= buttonX + BUTTON_WIDTH &&
           mouseY >= buttonY &&
           mouseY <= buttonY + BUTTON_HEIGHT;

       // Set button color based on hover state
       ALLEGRO_COLOR color = isHovered ?
           buttonHoverColor :
           buttonColor;

       // Draw the button rectangle
       al_draw_filled_rectangle(buttonX, buttonY,
                                buttonX + BUTTON_WIDTH,
                                buttonY + BUTTON_HEIGHT,
                                color);

       // Draw button text vertically centered
       int textHeight = al_get_font_line_height(font);
       int textY = buttonY + (BUTTON_HEIGHT - textHeight) / 2;

       al_draw_text(font, textColor,
                    buttonX + TEXT_PADDING_X, textY,
                    ALLEGRO_ALIGN_LEFT, menuOptions[i]);
   }
}

void draw_difficulty_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Draw logo
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Draw centered title
    al_draw_text(fontTitle, buttonColor, VIRTUAL_W / 2, 200, ALLEGRO_ALIGN_CENTER, "Game difficult");

    // Define square difficulty buttons
    const int BUTTON_SIZE = 150;
    const int BUTTON_SPACING = 30;
    const int NUM_BUTTONS = 3;
    const char* difficulties[] = { "EASY", "MEDIUM", "HARD" };

    // Calculate total width and starting X for horizontal centering
    int totalWidth = NUM_BUTTONS * BUTTON_SIZE + (NUM_BUTTONS - 1) * BUTTON_SPACING;
    int startX = (VIRTUAL_W - totalWidth) / 2;
    int buttonY = VIRTUAL_H / 2;

    int i;
    for (i = 0; i < NUM_BUTTONS; i++) {
        int buttonX = startX + i * (BUTTON_SIZE + BUTTON_SPACING);

        // Hover detection
        bool isHovered =
            mouseX >= buttonX &&
            mouseX <= buttonX + BUTTON_SIZE &&
            mouseY >= buttonY &&
            mouseY <= buttonY + BUTTON_SIZE;

        ALLEGRO_COLOR color = isHovered ? buttonHoverColor : buttonColor;

        // Draw square button
        al_draw_filled_rectangle(buttonX, buttonY,
                                 buttonX + BUTTON_SIZE,
                                 buttonY + BUTTON_SIZE,
                                 color);

        // Draw centered difficulty text
        int textWidth = al_get_text_width(font, difficulties[i]);
        int textHeight = al_get_font_line_height(font);
        int textX = buttonX + (BUTTON_SIZE - textWidth) / 2;
        int textY = buttonY + (BUTTON_SIZE - textHeight) / 2;

        al_draw_text(font, textColor, textX, textY, ALLEGRO_ALIGN_LEFT, difficulties[i]);
    }

    //
    // Draw BACK button centered below difficulty buttons
    //
    const int BACK_BUTTON_WIDTH = 150;
    const int BACK_BUTTON_HEIGHT = 40;
    const int BACK_BUTTON_SPACING_Y = 40;

    int backButtonX = (VIRTUAL_W - BACK_BUTTON_WIDTH) / 2;
    int backButtonY = buttonY + BUTTON_SIZE + BACK_BUTTON_SPACING_Y;

    bool isBackHovered =
        mouseX >= backButtonX &&
        mouseX <= backButtonX + BACK_BUTTON_WIDTH &&
        mouseY >= backButtonY &&
        mouseY <= backButtonY + BACK_BUTTON_HEIGHT;

    ALLEGRO_COLOR backColor = isBackHovered ? buttonHoverColor : buttonColor;

    al_draw_filled_rectangle(backButtonX, backButtonY,
                             backButtonX + BACK_BUTTON_WIDTH,
                             backButtonY + BACK_BUTTON_HEIGHT,
                             backColor);

    int backTextWidth = al_get_text_width(font, "BACK");
    int backTextHeight = al_get_font_line_height(font);
    int backTextX = backButtonX + (BACK_BUTTON_WIDTH - backTextWidth) / 2;
    int backTextY = backButtonY + (BACK_BUTTON_HEIGHT - backTextHeight) / 2;

    al_draw_text(font, textColor, backTextX, backTextY, ALLEGRO_ALIGN_LEFT, "BACK");
}

void draw_game_room(int mouseX, int mouseY) {
    init_color();
    al_clear_to_color(backgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    // Título
    al_draw_text(fontTitle, buttonColor, 30, 100, ALLEGRO_ALIGN_LEFT, "sCdoku");

    // Tamanho da grade e células
    const int GRID_SIZE = 9;
    const int CELL_SIZE = 50;
    const int GRID_PIXEL_SIZE = GRID_SIZE * CELL_SIZE;

    // Calcular posição centralizada
    int startX = (VIRTUAL_W - GRID_PIXEL_SIZE) / 2;
    int startY = (VIRTUAL_H - GRID_PIXEL_SIZE) / 2;

    int row, col;
    // Desenhar células
    for (row = 0; row <= GRID_SIZE; row++) {
        for (col = 0; col <= GRID_SIZE; col++) {
            int x = startX + col * CELL_SIZE;
            int y = startY + row * CELL_SIZE;

            // Linhas horizontais
            if (row < GRID_SIZE) {
                ALLEGRO_COLOR lineColor = (row % 3 == 0) ? textColor : al_map_rgb(200, 200, 200);
                float thickness = (row % 3 == 0) ? 2.0 : 1.0;
                al_draw_line(startX, y, startX + GRID_PIXEL_SIZE, y, lineColor, thickness);
            }

            // Linhas verticais
            if (col < GRID_SIZE) {
                ALLEGRO_COLOR lineColor = (col % 3 == 0) ? textColor : al_map_rgb(200, 200, 200);
                float thickness = (col % 3 == 0) ? 2.0 : 1.0;
                al_draw_line(x, startY, x, startY + GRID_PIXEL_SIZE, lineColor, thickness);
            }
        }
    }

    // Borda externa inferior e direita
    al_draw_line(startX, startY + GRID_PIXEL_SIZE, startX + GRID_PIXEL_SIZE, startY + GRID_PIXEL_SIZE, textColor, 2.0);
    al_draw_line(startX + GRID_PIXEL_SIZE, startY, startX + GRID_PIXEL_SIZE, startY + GRID_PIXEL_SIZE, textColor, 2.0);
}
