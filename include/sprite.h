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
    SDL_Rect box;
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
struct sprite* sprite_create(struct spritesheet* spritesheet,
        int sprite_pos, int x, int y, unsigned int w, unsigned int h);


/*
 * Renders a sprite on the renderer of it's spritesheet
 *
 * @param sprite The sprite to render
*/
void sprite_render(struct sprite* sprite);


/*
 * Deletes a sprite. 
 * ATTENTION : spritesheet of the sprite is not freed with this function.
 * It must be done by the user since the sprites can share the same spritesheet
 *
 * @param sprite The sprite to delete
*/ 
void sprite_delete(struct sprite* sprite);

/*
 * Returns 1 if mouse is hovering the sprite. That is, the mouse coordinates are
 * included in the sprite box (SDL_Rect).
 *
 * @param sprite The sprite to check
 *
 * @return 1 if mouse is hovering, 0 if not hovering, -1 if sprite is NULL
*/
int sprite_is_hovered(struct sprite* sprite);

#endif
