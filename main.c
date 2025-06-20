#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "src/states/room.h"
#include "src/draw/draw_rooms.h"
#include "src/control/handle.h"
#include "src/config.h"

GameRoom current_room;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *virtual_screen = NULL;

bool init_allegro();
void init_vars();
void register_events();

int main(int argc, char **argv)
{
    if(!init_allegro()) return -1;
    
    bool redraw = true;         //      Redraw is used to prevent unnecessary visual updates
    int mouseX = 0, mouseY = 0;               // physical mouse position (real window coords)
    int logicalMouseX = 0, logicalMouseY = 0; // logical mouse position (scaled to virtual coords)
    bool is_fullscreen = false;               // fullscreen state   

    GameRoom current_room = ROOM_MENU;        // current room
    
    while (1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;         //      Close window event

        if (ev.type == ALLEGRO_EVENT_TIMER)
            redraw = true; //      Timer event signals it's time to redraw

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            mouseX = ev.mouse.x;
            mouseY = ev.mouse.y;    //      Track mouse movement events, store physical mouse position
        }

        //      Handle events based on current room
        switch(current_room) {
            case ROOM_MENU:         //      Menu room
                if(!handle_menu_events(ev, logicalMouseX, logicalMouseY, &current_room)){
                    al_destroy_display(display);
                    al_destroy_event_queue(event_queue);
                    al_destroy_timer(timer);
                    destroy_font();
                    return 0;
                }
                break;
        
            case ROOM_CONFIG:       //      Config room
                handle_config_events(ev, logicalMouseX, logicalMouseY, &current_room, &is_fullscreen);
                break;
        
            case ROOM_DIFFICULTY: { //      Difficulty room
                Difficulty selected_difficulty = handle_difficulty_events(ev, logicalMouseX, logicalMouseY, &current_room);
                break;
            }

            case ROOM_GAME:         //      Game room
                handle_game_events(ev, logicalMouseX, logicalMouseY, &current_room);
                break;
        }

        //      Redraw only if needed and event queue is empty
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            int disp_w = al_get_display_width(display);            //      Get the real display size (in pixels)
            int disp_h = al_get_display_height(display);           //      Get the real display size (in pixels)
            
            float scaleX = (float)disp_w / VIRTUAL_W;              //      Calculate scale to fit virtual screen into display keeping aspect ratio 4:3
            float scaleY = (float)disp_h / VIRTUAL_H;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;     //      Uniform scale to fit
            
            int bar_w = (int)((disp_w - VIRTUAL_W * scale) / 2);   //      Calculate black bars (letterboxing) offsets
            int bar_h = (int)((disp_h - VIRTUAL_H * scale) / 2);

            logicalMouseX = (int)((mouseX - bar_w) / scale);       //      Adjust mouse position to virtual coordinates, considering black bars and uniform scale
            logicalMouseY = (int)((mouseY - bar_h) / scale);

            // Clamp mouse inside virtual viewport
            if (logicalMouseX < 0) logicalMouseX = 0;
            if (logicalMouseX > VIRTUAL_W) logicalMouseX = VIRTUAL_W;
            if (logicalMouseY < 0) logicalMouseY = 0;
            if (logicalMouseY > VIRTUAL_H) logicalMouseY = VIRTUAL_H;

            //      Start drawing on the virtual bitmap (logical resolution)
            al_set_target_bitmap(virtual_screen);
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //      Draw a red circle at the logical mouse position (debugging)
            al_draw_filled_circle(logicalMouseX, logicalMouseY, 5, al_map_rgb(255, 0, 0));

            //      Draw according to the current game room
            switch (current_room)
            {
                case ROOM_INTRO:        //      Intro room
                    // Intro Animation and start (not implemented)
                    break;

                case ROOM_MENU:         //      Menu room
                    draw_menu_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_DIFFICULTY:   //      Difficulty room
                    draw_difficulty_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_CONFIG:       //      Config room
                    draw_config_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_GAME:         //      Game room
                    draw_game_room(logicalMouseX, logicalMouseY);
                    break;
            }

            al_set_target_bitmap(al_get_backbuffer(display));   //      Switch back to drawing on the actual display backbuffer
            al_clear_to_color(al_map_rgb(0, 0, 0));             //      Clear the display to black (for letterbox bars)
            al_draw_scaled_bitmap(virtual_screen,0, 0, VIRTUAL_W, VIRTUAL_H,bar_w, bar_h, VIRTUAL_W * scale, VIRTUAL_H * scale,0);
            al_flip_display();
        }
    }

    //      Destroy Allegro objects
    al_destroy_bitmap(virtual_screen);
    destroy_font();
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);


    return 0;
}


bool init_allegro() {
    //      Basic Allegro initialization and verifications
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro core!\n");
        return false;
    }

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_NO_PRESERVE_TEXTURE);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    al_init_font_addon();

    if (!al_init_image_addon()) {
        fprintf(stderr, "Failed to initialize image addon!\n");
        return false;
    }

    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Failed to initialize TTF addon!\n");
        return false;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to initialize primitives addon!\n");
        return false;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Failed to install keyboard!\n");
        return false;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Failed to install mouse!\n");
        return false;
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);

    init_vars();
    register_events();

    al_start_timer(timer);
    return true;
}

void init_vars() {
    display = al_create_display(VIRTUAL_W, VIRTUAL_H);
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    virtual_screen = al_create_bitmap(VIRTUAL_W, VIRTUAL_H);
    al_set_window_position(display, 0, 0);
    init_font();
    init_color();
}

void register_events(){
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());    
}
