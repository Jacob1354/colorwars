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


/*
 * Creates a game instance
 *
 * @param width Width of the grid (in box els)
 * @param height Height of the grid (in box els)
 * @param players_nb Quantity of players playing
 * @param bots_nb Quantity of bots playing
 *
 * @return Pointer to the game instance or NULL if there was an error
*/
struct game* game_create(int width, int height, int players_nb, int bots_nb);





#endif
