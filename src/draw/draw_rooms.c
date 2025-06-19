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
