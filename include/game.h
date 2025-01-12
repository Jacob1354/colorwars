#ifndef GAME_H
#define GAME_H
#include "sdl2.h"

// --------------- //
// Data structures //
// --------------- //

enum gamestate {
    GAME_STATE_PLAY,
    GAME_STATE_GAME_OVER,
    GAME_STATE_QUIT
};

enum player {
    PINK,
    CYAN,
    GREEN,
    YELLOW,
    NONE
};

struct box {
    int x;                  // Board relative x
    int y;                  // Board relative y
    enum player;            // Player owning the box
    int points;             // Nb of dots of the box 
    struct srite* sprite    // Sprite of the box
};

struct game {
    enum gamestate gamestate;
    int width;
    int height;
    int players_nb;
    int bots_nb;
    enum player player_turn;
    struct box** grid;
    SDL_Renderer* renderer;
};


// --------- //
// Functions //
// --------- //


#endif
