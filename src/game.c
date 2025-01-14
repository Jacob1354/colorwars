#include <stdio.h>
#include <stdlib.h>
#include "../include/game.h"
#include "../include/constants.h"
#include "../include/spritesheet.h"
#include "../include/img_reader.h"


//Game creation functinos
//==========================================================

//Initialize the game grid. Returns 1 if worked, -1 if game was NULL, 0
//otherwise
int init_grid(struct game* game); 
int fill_grid(struct game* game, struct spritesheet* spritesheet); 
struct box* box_create(int x, int y, enum player owner, 
        int points, struct sprite* sprite);
void box_free(struct box* box);


//Game run functions
//==========================================================
void event_loop(struct game* game);
void game_click(struct game* game);
void game_render(struct game* game);
void game_set_background_color(struct game* game, Uint8* r, Uint8* g, Uint8* b);
int game_render_grid(struct game* game);


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
    game->renderer = renderer;
    if(init_grid(game) == 1) { 
        game->state = GAME_STATE_PLAY;
        game->player_turn = PLAYER_PINK;
        game->width = width;
        game->height = height;
        game->players_nb = players_nb;
        game->bots_nb = bots_nb;
    }
    return game;
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
    if(game->renderer == NULL) {
        printf("game.c:init_grid: game renderer is NULL\n");
        return 0;
    }
    struct spritesheet* ss = spritesheet_create(GAME_BOX_SPRITE_PATH,
            GAME_BOX_SPRITE_NUM_ROWS, GAME_BOX_SPRITE_NUM_COLS, 
            GAME_BOX_SPRITE_NUM_SPRITES, game->renderer);
    if(ss == NULL)
        return 0;
    return fill_grid(game, ss); 
}

int fill_grid(struct game* game, struct spritesheet* ss) {
    unsigned int dim[2];
    if(get_png_dimensions(GAME_BOX_SPRITE_PATH, dim) != 1)
        return 0;
    dim[0] = dim[0]/GAME_BOX_SPRITE_NUM_COLS;
    dim[1] = dim[1]/GAME_BOX_SPRITE_NUM_ROWS;
    int x_og = (WND_W - game->width * dim[0])/2;
    int y_og = (WND_H - game->height * dim[1])/2;
    int i;
    struct sprite* sprite;
    for(i = 0; i < GAME_BOARD_H * GAME_BOARD_W; i++) {
        sprite = sprite_create(ss, 0,
                x_og + (i - i/GAME_BOARD_W * dim[0]),
                y_og + (i/GAME_BOARD_W * dim[1]),
                dim[0], dim[1]);
        if(sprite == NULL)
            return 0;
        game->grid[i] = box_create(i - i/GAME_BOARD_W,
                i/GAME_BOARD_W, PLAYER_NONE, 0, sprite);
        if(game->grid[i] == NULL)
            return 0;
    }
    return 1;
}

struct box* box_create(int x, int y, enum player owner, 
        int points, struct sprite* sprite) {
    struct box* box = malloc(sizeof(struct box));
    if(box == NULL) {
        printf(ERR_MSG_ALLOC, "struct box");
        return NULL;
    }
    box->x = x;
    box->y = y;
    box->player = owner;
    if(points < 0) points = 0;
    box->points = points % GAME_BOX_MAX_POINTS;
    box->sprite = sprite;
    return box;
}


void game_run(struct game* game) {
    if(game != NULL) {
        while(game->state != GAME_STATE_QUIT && game->state != GAME_STATE_FORCE_QUIT) {
            event_loop(game);
            game_render(game);
        }
    }
}

void event_loop(struct game* game) {
    if(game != NULL) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case(SDL_QUIT):
                    game->state = GAME_STATE_FORCE_QUIT;
                    break;
                case(SDL_MOUSEBUTTONDOWN):
                    if(e.button.button == SDL_BUTTON_LEFT)
                        game->state = GAME_STATE_QUIT;//game_click(game);
                default:
            }
        }
    }
}

void game_render(struct game* game) {
    if(game != NULL) {
        Uint8 r, g, b;
        game_set_background_color(game, &r, &g, &b);
        SDL_SetRenderDrawColor(game->renderer, r, g, b, 255);
        SDL_RenderClear(game->renderer);
        if(game_render_grid(game) != 1)
            printf("Unable to properly render game grid\n");
    }
}

void game_set_background_color(struct game* game, Uint8* r, Uint8* g, Uint8* b) {
    if(r != NULL && g != NULL && b != NULL) {
        switch(game->player_turn) {
                case PLAYER_PINK:
                    *r = 244;
                    *g = 20;
                    *b = 172;
                    break;
                case PLAYER_CYAN:
                    *r = 4;
                    *g = 252;
                    *b = 244;
                    break;
                case PLAYER_GREEN:
                    *r = 66;
                    *g = 229;
                    *b = 40;
                    break;
                case PLAYER_YELLOW:
                    *r = 228;
                    *g = 212;
                    *b = 44;
                    break;
                default:
        }
    }
}

int game_render_grid(struct game* game) {
    int return_code = 1;
    if(game != NULL && game->grid != NULL) {
        int i;
        for(i = 0; i < game->width * game->height; i++) {
            if(game->grid[i] != NULL && game->grid[i]->sprite != NULL)
                sprite_render(game->grid[i]->sprite);
            else { 
                printf("game.c::game_render_grid:"
                        "Unable to render sprite #%d\n", i);
                return_code = 0;
            }
        }
    } else return_code = -1;
    return return_code;
}

void game_delete(struct game* game) {
    if(game != NULL) {
        if(game->grid != NULL) {
            int i = 0;
            int spritesheet_deleted = 0;
            //deletes spritesheet
            while(i < game->width * game->height
                    && !spritesheet_deleted) {
                if(game->grid[i] != NULL) {
                    if(game->grid[i]->sprite != NULL) {
                        if(game->grid[i]->sprite->spritesheet != NULL) {
                            spritesheet_delete(
                                    game->grid[i]->sprite->spritesheet
                                    );
                            spritesheet_deleted = 1;
                        }
                    }
                }
            }
            //frees grid's boxes
            for(i = 0; i < game->width * game->height; i++)
                if(game->grid[i] != NULL) 
                    box_free(game->grid[i]);
        }
    }
    free(game);
}

void box_free(struct box* box) {
    if(box != NULL && box->sprite != NULL) {
        sprite_delete(box->sprite);
    }
    free(box);
}

















