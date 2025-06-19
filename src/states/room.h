#ifndef ROOM_H
#define ROOM_H

//
//  State room enum
//
typedef enum {
    ROOM_INTRO,
    ROOM_MENU,
    ROOM_CONFIG,
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

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} Difficulty;

#endif