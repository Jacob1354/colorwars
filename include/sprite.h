#ifndef SPRITE_H
#define SPRITE_H

#include "spritesheet.h"

// --------------- //
// Data structures //
// --------------- //


struct sprite {
    struct spritesheet* spritesheet;
    int sprite_nb;
    int x;
    int y;
};


// --------- //
// Functions //
// --------- //

/*
 * Creates a new sprite
 *
 * @param spritesheet The spritesheet from where to get it
 * @param sprite_nb The position of the sprite in the spritesheet
 * @param x X postion of the top left corner of the sprite in the window
 * @param y Y postion of the top left corner of the sprite in the window
*/
struct sprite* sprite_init(struct spritesheet* spritesheet,
        int sprite_nb, int x, int y);

/*
 * Renders a sprite on the renderer of it's spritesheet
 *
 * @param sprite The sprite to render
*/
void sprite_render(struct sprite* sprite);



/*
 * Deletes a sprite
 *
 * @param sprite The sprite to delete
*/ 
void sprite_delete(struct sprite* sprite);



#endif
