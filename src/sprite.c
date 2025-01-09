#include <stdio.h>
#include "../include/sprite.h"
#include "../include/constants.h"


struct sprite* sprite_create(struct spritesheet* spritesheet,
        int sprite_pos, int x, int y, unsigned int w, unsigned int h) {
    struct sprite* sprite = malloc(sizeof(struct sprite));
    if(sprite == NULL) {
        printf(ERR_MSG_ALLOC, "sprite");
        return NULL;
    }
    if(spritesheet == NULL) {
        printf("Error, can't create a sprite with an empty spritesheet");
        return NULL;
    }
    sprite->spritesheet = spritesheet;
    sprite->sprite_pos = sprite_pos;
    sprite->x = x;
    sprite->y = y;
    sprite->w = w;
    sprite->h = h;
    return sprite;
}


void sprite_render(struct sprite* sprite) {
    if(sprite != NULL)
        spritesheet_render(sprite->spritesheet,
                sprite->x,
                sprite->y,
                255,
                sprite->sprite_pos
                );
}


void sprite_delete(struct sprite* sprite) {
    spritesheet_delete(sprite->spritesheet);
    free(sprite);
}
