#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "src/game/sudoku.h"
#include "src/states/states.h"
#include "src/draw/draw_rooms.h"
#include "src/control/handle.h"
#include "src/config.h"
#include "src/game/game.h"

#define SIZE 9

#ifdef _WIN32
#include "src/online/online_win.h"
#else
#include "src/online/online_unix.h"
#endif

// Global variables for game state management
GameRoom current_room;  // Current game room state

// Allegro resources
ALLEGRO_DISPLAY *display = NULL;        // Main display window
ALLEGRO_EVENT_QUEUE *event_queue = NULL; // Event queue for handling user input
ALLEGRO_TIMER *timer = NULL;            // Timer for game loop
ALLEGRO_BITMAP *virtual_screen = NULL;  // Virtual screen for scaling

// Function declarations
bool init_allegro();     // Initialize Allegro library and resources
void init_vars();        // Initialize game variables
void register_events();  // Register event handlers

// Main function - Game entry point
int main(int argc, char **argv)
{
    printf("voce quer jogar online? (1 = sim, 0 = nao)\n");
    int online = 0;
    scanf("%d", &online);
    char ip[15];

    ON_SOCK opponent;
    bool is_admin = false;
    if (online) {
        printf("entre com o ip do seu oponente ex: 127.0.0.1\n");
        scanf("%s", ip);
        opponent = connect_to(ip);
        printf("opponent: %d\n", opponent);
        if (opponent==(-1)) {
            printf("admin\n");
            opponent = get_oponnent(); // blocking
            is_admin = true;
        }

    } else {
        opponent = -1;
        is_admin = true;
    }


    if(!init_allegro()) return -1;

    // Game state variables
    bool redraw = true;         // Flag to indicate when screen needs updating
    int mouseX = 0, mouseY = 0;               // Physical mouse position (window coordinates)
    int logicalMouseX = 0, logicalMouseY = 0; // Logical mouse position (virtual coordinates)
    bool is_fullscreen = false;               // Fullscreen mode flag


    // Initialize game state
    current_room = ROOM_MENU;        // Start in menu room
    GameState gameState = { -1, -1 , {{false}}}; // Initialize game state
    Game game;

    bool done = false;
    if (!is_admin) {
        // o servo espera o admin entrar no ROOM_GAME e n decide nada
        char b[SIZE*SIZE*2]; // vai receber 2 tabs um sendo o gab
        online_recv(opponent, b, SIZE*SIZE*2); // blocking read
        game.b = create_board(SIZE);
        game.gab = create_board(SIZE);
        game.size = SIZE;
        int c = 0;
        int i, j;
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                if (b[i*SIZE + j] == EMPTY) c++;
                game.b[i][j] = b[i*SIZE + j];
            }
        }
        game.left = c;

        Board gab;
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                game.gab[i][j] = b[(SIZE*SIZE) + i*SIZE + j];
            }
        }
        current_room = ROOM_GAME;

        done = true;
    }

    // Main game loop
    char opmsg[3] = {0};
    while (1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);


        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;         // Close window event

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (online && done && should_read(opponent)) {
                online_recv(opponent, opmsg, 3);
                if (strncmp(opmsg, "val", 3) == 0) {
                    printf("o oponente acertou uma\n");
                    memset(opmsg, 0, 3);
                }
            }
            redraw = true; // Timer event signals it's time to redraw
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            mouseX = ev.mouse.x;
            mouseY = ev.mouse.y;    // Track mouse movement events, store physical mouse position
        }

        // Handle events based on current room
        switch(current_room) {
            case ROOM_MENU:         // Menu room
                if(!handle_menu_events(ev, logicalMouseX, logicalMouseY, &current_room)){
                    al_destroy_display(display);
                    al_destroy_event_queue(event_queue);
                    al_destroy_timer(timer);
                    destroy_font();
                    return 0;
                }
                break;

            case ROOM_CONFIG:       // Config room
                handle_config_events(ev, logicalMouseX, logicalMouseY, &current_room, &is_fullscreen);
                break;

            case ROOM_DIFFICULTY:
            {                       // Difficulty room
                Difficulty selected_difficulty = handle_difficulty_events(ev, logicalMouseX, logicalMouseY, &current_room);
                if (selected_difficulty != DIFFICULTY_NONE) {
                    int to_remove;
                    switch (selected_difficulty) {
                        case DIFFICULTY_EASY: to_remove = 30; break;
                        case DIFFICULTY_MEDIUM: to_remove = 45; break;
                        case DIFFICULTY_HARD: to_remove = 60; break;
                        default: to_remove = 45;
                    }

                    game = new_game(SIZE, to_remove);

                    if(is_admin) {
                        char msg[SIZE*SIZE*2];
                        int i,j;
                        for (i = 0; i < SIZE; i++) {
                            for (j = 0; j < SIZE; j++) {
                                msg[i*SIZE + j] = game.b[i][j];
                            }
                        }
                        for (i = 0; i < SIZE; i++) {
                            for (j = 0; j < SIZE; j++) {
                                msg[(SIZE*SIZE) + i*SIZE + j] = game.gab[i][j];
                            }
                        }
                        online_send(opponent, msg, SIZE*SIZE*2);

                        done = true;
                    }

                    selected_difficulty = DIFFICULTY_NONE;
                    current_room = ROOM_GAME;
                }
                break;
            }

            case ROOM_GAME:
            {                       // Game room
                handle_game_events(ev, logicalMouseX, logicalMouseY, &gameState, &game, opponent);
                break;
            }
        }

        // Redraw only if needed and event queue is empty
        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            // Get display dimensions
            int disp_w = al_get_display_width(display);
            int disp_h = al_get_display_height(display);

            // Calculate scaling factors for maintaining aspect ratio
            float scaleX = (float)disp_w / VIRTUAL_W;
            float scaleY = (float)disp_h / VIRTUAL_H;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            // Calculate letterbox bars
            int bar_w = (int)((disp_w - VIRTUAL_W * scale) / 2);
            int bar_h = (int)((disp_h - VIRTUAL_H * scale) / 2);

            // Convert mouse position to virtual coordinates
            logicalMouseX = (int)((mouseX - bar_w) / scale);
            logicalMouseY = (int)((mouseY - bar_h) / scale);

            // Ensure mouse stays within virtual viewport
            if (logicalMouseX < 0) logicalMouseX = 0;
            if (logicalMouseX > VIRTUAL_W) logicalMouseX = VIRTUAL_W;
            if (logicalMouseY < 0) logicalMouseY = 0;
            if (logicalMouseY > VIRTUAL_H) logicalMouseY = VIRTUAL_H;

            // Draw to virtual screen
            al_set_target_bitmap(virtual_screen);
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Draw current game room
            switch (current_room)
            {
                case ROOM_INTRO:        // Intro room
                    // TODO: Implement intro animation
                    break;

                case ROOM_MENU:         // Menu room
                    draw_menu_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_DIFFICULTY:   // Difficulty selection room
                    draw_difficulty_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_CONFIG:       // Configuration room
                    draw_config_room(logicalMouseX, logicalMouseY);
                    break;

                case ROOM_GAME:         // Game room
                    draw_game_room(logicalMouseX, logicalMouseY, &gameState, &game);
                    break;
            }

            // Draw scaled virtual screen to display
            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(virtual_screen, 0, 0, VIRTUAL_W, VIRTUAL_H,
                                 bar_w, bar_h, VIRTUAL_W * scale, VIRTUAL_H * scale, 0);
            al_flip_display();
        }
    }

    // Clean up resources
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

    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED);

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
