#ifndef ROOM_H
#define ROOM_H

//
//  State room enum
//
typedef enum {
    ROOM_INTRO,
    ROOM_MENU,
    ROOM_CONFIG,
    ROOM_GAME,
    ROOM_DIFFICULTY
} GameRoom;


typedef enum {
    DIFFICULTY_NONE = -1, // just for default value
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM = 1,
    DIFFICULTY_HARD = 2
} Difficulty;

#endif