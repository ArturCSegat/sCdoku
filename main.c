#include <stdio.h>

// Allegro includes
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
// Game source files
#include "src/states/room.h"
#include "src/game.c"
#include "src/utils.c"
// Screen and FPS constants
const float FPS = 60;
const int SCREEN_W = 1024;
const int SCREEN_H = 768;

int main(int argc, char **argv)
{
    //
    //      Basic Allegro initialization
    //
    al_init();                        // Initialize core Allegro
    al_init_font_addon();            // Built-in font support
    al_init_image_addon();           // Image loading support
    al_init_primitives_addon();      // Drawing primitives (rects, lines, etc.)
    al_install_keyboard();           // Keyboard input
    al_install_mouse();              // Mouse input

    //
    //      Display and core object creation
    //
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED); // Must be set BEFORE display creation
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);   // Create the window
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();        // Create the event queue
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);                 // Timer for consistent FPS
    ALLEGRO_FONT *font = al_create_builtin_font();                     // Built-in default font

    // Set render target to the screen's backbuffer
    al_set_target_bitmap(al_get_backbuffer(display));

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    //
    //      Initial screen clear
    //
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);

    //
    //      Redraw is used to prevent unnecessary visual updates
    //
    bool redraw = true;

    //
    //      Click flag tracks whether the screen was clicked
    //      savebvx and savebvy store mouse coordinates
    //
    int click = 0;
    int savebvx;
    int savebvy;

    //
    //      Game vars
    //
    
    // saves the current room the game is in
    GameRoom actual_room = ROOM_MENU;
    // saves the current menu option the game is in
    MenuOption actual_menu_option = MENU_OPTION_START;
    
    while (1)
    {
        //
        //      Event listener
        //
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //      Close window event
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        //      Timer event signals it's time to redraw
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }

        //      Redraw only if needed and event queue is empty
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_flip_display();
        }
    }

    //
    //      Destroy Allegro objects
    //
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
