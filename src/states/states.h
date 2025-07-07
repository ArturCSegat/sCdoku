#ifndef STATES_H
#define STATES_H

#ifdef _WIN32
#include "../online/online_win.h"
#else
#include "../online/online_unix.h"
#endif

// Game room enum
typedef enum {
    ROOM_MENU,
    ROOM_CONFIG,
    ROOM_IP,
    ROOM_WAITING,
    ROOM_GAME,
    ROOM_DIFFICULTY,
    ROOM_VICTORY,
    ROOM_LOSE
} GameRoom;

// Difficulty enum
typedef enum {
    DIFFICULTY_NONE = -1,
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM = 1,
    DIFFICULTY_HARD = 2
} Difficulty;

// Game state struct
typedef struct {
    bool isOnline;
    int selectedRow;
    int selectedCol;
    bool errors[9][9];
    char attempts[9][9];
    double startTime;
} GameState;

// Online state struct
typedef struct {
    ON_SOCK opponent;
    bool is_admin;
    char ip[15];
    bool done;
    bool ip_invalid;
    bool waiting;
} OnlineState;

#endif
