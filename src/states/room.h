#ifndef STATE_H
#define STATE_H

//
//  State room enum
//
typedef enum {
    ROOM_INTRO,
    ROOM_MENU,
    ROOM_GAME
} GameRoom;

//
// State menu vars basic
//
typedef enum {
    MENU_OPTION_START,
    MENU_OPTION_CONFIG,
    MENU_OPTION_EXIT
} MenuOption;

#endif