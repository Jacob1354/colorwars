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
    sprite->box.x = x;
    sprite->box.y = y;
    sprite->box.w = w/spritesheet->num_columns;
    sprite->box.h = h/spritesheet->num_rows;
    return sprite;
}


void sprite_render(struct sprite* sprite) {
    if(sprite != NULL)
        spritesheet_render(sprite->spritesheet,
                sprite->box.x,
                sprite->box.y,
                255,
                sprite->sprite_pos
                );
}


void sprite_delete(struct sprite* sprite) {
    free(sprite);
}


int sprite_is_hovered(struct sprite* sprite) {
    if(sprite == NULL) {
        printf("Sprite is NULL\n");
        return -1;
    }
    int x, y;
    SDL_Rect box = sprite->box;
    SDL_GetMouseState(&x, &y);
    return (box.x <= x && x <= box.x + box.w)
        && (box.y <= y && y <= box.y + box.h)
        ? 1 : 0;
}




