#ifndef STATES_H
#define STATES_H

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
    DIFFICULTY_EASY = 0,        // 42 dicas
    DIFFICULTY_MEDIUM = 1,      // 50 dicas
    DIFFICULTY_HARD = 2         // 57 dicas
} Difficulty;

typedef struct {
    int selectedRow;
    int selectedCol;
    bool errors[9][9];
    char attempts[9][9];
} GameState;

#endif