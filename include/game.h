#ifndef GAME_H
#define GAME_H
#include "sdl2.h"

// --------------- //
// Data structures //
// --------------- //

enum gamestate {
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER,
    GAME_STATE_QUIT
};

enum player {
    NONE,
    PINK,
    CYAN,
    GREEN,
    YELLOW
};

struct box {
    int x;
    int y;
    enum player;
    int points;
};

struct game {
    enum gamestate gamestate;
    int width;
    int height;
    int nb_of_players;
    enum player player_turn;
    struct box** boxes;
    SDL_Renderer* renderer;
};


// --------- //
// Functions //
// --------- //


#endif
