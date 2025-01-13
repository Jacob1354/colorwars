#ifndef GAME_H
#define GAME_H
#include "sdl2.h"
#include "sprite.h"

// --------------- //
// Data structures //
// --------------- //

enum gamestate {
    GAME_STATE_PLAY,
    GAME_STATE_GAME_OVER,
    GAME_STATE_QUIT,
    GAME_STATE_FORCE_QUIT
};

enum player {
    PLAYER_PINK,
    PLAYER_CYAN,
    PLAYER_GREEN,
    PLAYER_YELLOW,
    PLAYER_NONE
};

struct box {
    int x;                      // Board relative x
    int y;                      // Board relative y
    enum player player;         // Player owning the box
    int points;                 // Nb of dots of the box 
    struct sprite* sprite;      // Sprite of the box
};

struct game {
    enum gamestate state;
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
struct game* game_create(int width, int height, int players_nb, 
        int bots_nb, SDL_Renderer* renderer);

/*
 * Runs the game passed as arg
 *
 * @param game The game to run
*/
void game_run(struct game* game);


/*
 * Deletes a game struct
 *
 * @param game The game to delete
*/
void game_delete(struct game* game);

#endif
