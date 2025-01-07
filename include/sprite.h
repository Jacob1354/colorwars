#ifndef SPRITE_H
#define SPRITE_H

#include "spritesheet.h"

// --------------- //
// Data structures //
// --------------- //


struct sprite {
    int id;
    struct spritesheet* spritesheet;
    int sprite_pos;
    int x;
    int y;
    unsigned int w;
    unsigned int h;
};


// --------- //
// Functions //
// --------- //

/*
 * Creates a new sprite
 *
 * @param spritesheet The spritesheet from where to get it
 * @param sprite_pos The position of the sprite in the spritesheet (begins at 0)
 * @param x X postion of the top left corner of the sprite in the window
 * @param y Y postion of the top left corner of the sprite in the window
*/
struct sprite* sprite_init(struct spritesheet* spritesheet,
        int sprite_pos, int x, int y, unsigned int w, unsigned int h);

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
