#include <stdio.h>
#include <stdib.h>
#include "../include/game.h"
#include "../include/constants.h"



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
    game->grid = init_grid(game);
    if(game->grid != NULL) {
        game->state = GAME_STATE_PLAY;
        game->player_turn = PINK;
        game->width = width;
        game->height = height;
        game->players_nb = players_nb;
        game->bots_nb = bots_nb;
        game->renderer = renderer;
    }
}



void game_run(struct game* game) {

}
void game_delete(struct game* game) {

}

