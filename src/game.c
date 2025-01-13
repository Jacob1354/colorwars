#include <stdio.h>
#include <stdib.h>
#include "../include/game.h"
#include "../include/constants.h"
#include "../include/spritesheet"
#include "../include/img_reader.h"


//Game creation functinos
//==========================================================

//Initialize the game grid. Returns 1 if worked, -1 if game was NULL, 0
//otherwise
int init_grid(struct game* game); 
int fill_grid(struct game* game, struct spritesheet* spritesheet, int dim[2]);



struct game* game_create(int width, int height, int players_nb, 
        int bots_nb, SDL_Renderer* renderer) {
    struct game* game = malloc(sizeof(struct game));
    if(game == NULL) {
        printf(ERR_MSG_ALLOC, "game struct");
        return NULL;
    }
    if(renderer == NULL) {
        printf("game_create : Rederer is null, unable to create game\n");
        return NULL;
    }
    if(init_grid(game) == 1) { 
        game->state = GAME_STATE_PLAY;
        game->player_turn = PLAYER_PINK;
        game->width = width;
        game->height = height;
        game->players_nb = players_nb;
        game->bots_nb = bots_nb;
        game->renderer = renderer;
    }
}

int init_grid(struct game* game) {
    if(game == NULL) {
        printf("game.c::init_grid: game struct is NULL\n"); 
        return -1;
    }
    game->grid = calloc(GAME_BOARD_W * GAME_BOARD_H, sizeof(struct box));
    if(game->grid == NULL) {
        printf(ERR_MSG_ALLOC, "game->grid");
        return 0;
    }
    unsigned int dim[2];
    struct spritesheet* ss = spritesheet_create(GAME_BOX_SPRITE_PATH,
            GAME_BOX_SPRITE_NUM_ROWS, GAME_BOX_SPRITE_NUM_COLS, 
            GAME_BOX_SPRITE_NUM_SPRITES, game->renderer);
    if(ss == NULL)
        return 0;
    if(get_png_dimensions(GAME_BOX_SPRITE_PATH, dim) != 1)
        return 0;
   return fill_grid(game, ss, dim); 
}

void game_run(struct game* game) {

}
void game_delete(struct game* game) {

}

















