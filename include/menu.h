#ifndef MENU_H
#define MENU_H

#include "sprite.h"
#include "sdl2.h"


// --------------- //
// Data structures //
// --------------- //

enum menu_state {
    MENU_STATE_NONE,
    MENU_STATE_PLAY,
    MENU_STATE_QUIT,
    MENU_STATE_PLAYERS_UP,
    MENU_STATE_PLAYERS_DOWN,
    MENU_STATE_BOTS_UP,
    MENU_STATE_BOTS_DOWN
};


struct menu {
    enum menu_state state;
    int players_nb;
    int bots_nb;
    struct sprite** sprites;
    int sprite_nb;
    SDL_Renderer* renderer;
};


// --------- //
// Functions //
// --------- //

/*
 * Creates a menu and returns it's pointer
 *
 * @param renderer The renderer of the menu
*/
struct menu* menu_init(SDL_Renderer* renderer);


/*
 * Runs the menu loop
 *
 * @param menu The menu to run
*/ 
void menu_run(struct menu* menu);


/*
 * Deletes the menu
 *
 * @param menu The menu to delete
*/ 
void menu_delete(struct menu* menu);



#endif
