#include <stdio.h>
#include <stdlib.h>
#include "../include/game.h"
#include "../include/constants.h"
#include "../include/spritesheet.h"
#include "../include/img_reader.h"

//Data structures
//==========================================================

//Game creation functinos
//==========================================================

//Fills game, return 1 if worked, 0 otherwise
int game_fill(struct game* game, SDL_Renderer* renderer, int width,
        int height, int players_nb, int bots_nb);
//Loads gameover sprite, return 1 if worked, 0 otherwise
int load_gameover(struct game* game, SDL_Renderer* renderer);
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
int find_box_hovered(struct game* game);
void update_grid(struct game* game, int box_index);
void update_box_sprite(struct box* box);
void next_player(struct game* game);
void explode_boxes(struct game* game, int* boxes_indexes, int box_count);

//Explodes a box of the game and increases next_boxes_count by the number
//of boxes touched by the exlosion that reached the max number of points or more
void explode_box(struct game* game, int box_index, 
        int* next_boxes_indexes, int* next_boxes_count);

//Applies consequences of a near box_eplosion : add 1 point(and block to max nb
//of point) and change player to current game_player. If max number of points is
//reached, then it is added to next_boxes_indexes and it increases the counter.
void update_box_from_collision(struct game* game, int box_index,
        int* next_boxes_indexes, int* next_boxes_count); 
void box_reset(struct box* box);
int game_ended(struct game* game);
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
    if(game_fill(game, renderer, width, height, players_nb, bots_nb) != 1)
        return NULL;
    return game;
}

int game_fill(struct game* game, SDL_Renderer* renderer, int width,
        int height, int players_nb, int bots_nb) {
    if(game == NULL) {
        printf("game.c::game_fill: game is NULL\n");
        return 0;
    }
    game->renderer = renderer;
    if(load_gameover(game, renderer) != 1)
        return 0;
    game->width = width;
    game->height = height;
    if(init_grid(game) == 1) { 
        game->state = GAME_STATE_PLAY;
        game->player_turn = PLAYER_PINK;
        game->turn = 1;
        game->players_nb = players_nb;
        game->bots_nb = bots_nb;
    } else return 0;
    return 1;
}

int load_gameover(struct game* game, SDL_Renderer* renderer) {
    if(game == NULL) {
        printf("game.c::load_gameover: game is NULL\n");
        return 0;
    }
    struct spritesheet* ss = spritesheet_create(
            GAME_GAMEOVER_SPRITE_PATH,
            GAME_GAMEOVER_SPRITE_NUM_ROWS,
            GAME_GAMEOVER_SPRITE_NUM_COLS,
            GAME_BOX_SPRITE_NUM_SPRITES,
            renderer);
    game->sprite_gameover = sprite_create(ss, 0, 0, 0, 
            GAME_BOARD_W, GAME_BOARD_H);
    return game->sprite_gameover != NULL;
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
    int x_0 = (WND_W - game->width * dim[0]/GAME_BOX_SPRITE_NUM_COLS)/2;
    int y_0 = (WND_H - game->height * dim[1]/GAME_BOX_SPRITE_NUM_ROWS)/2;
    int i;
    struct sprite* sprite;
    for(i = 0; i < GAME_BOARD_H * GAME_BOARD_W; i++) {
        sprite = sprite_create(ss, PLAYER_NONE * GAME_BOX_SPRITE_NUM_COLS,
                x_0 + //x_0 + x_pos_in_grid * box_width
                ((i - i/GAME_BOARD_H*GAME_BOARD_W) * dim[0]/GAME_BOX_SPRITE_NUM_COLS),
                y_0 + //y_0 + y_pos_in_grid * box_heigth
                ((i/GAME_BOARD_H) * dim[1]/GAME_BOX_SPRITE_NUM_ROWS),
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
                    if(e.button.button == SDL_BUTTON_LEFT) {
                        if(game->state == GAME_STATE_PLAY)
                            game_click(game);
                        else if(game->state == GAME_STATE_GAME_OVER)
                            game->state = GAME_STATE_QUIT;
                     }
                default:
            }
        }
    }
}

void game_click(struct game* game) {
    if(game != NULL) {
        if(game->grid != NULL) {
            int box_index = find_box_hovered(game);
            if(box_index >= 0) {
                update_grid(game, box_index);
                if((game->turn-1)/game->players_nb > 0 // Checks if first turn
                        && game_ended(game))
                    game->state = GAME_STATE_GAME_OVER;
            }

        } else printf("game.c:game_click: game->grid is NULL\n");
    } else printf("game.c:game_click: game is NULL\n");
}
int find_box_hovered(struct game* game) {
    int hovering = -1;
            int i = 0; 
    if(game != NULL) {
        if(game->grid != NULL) {
            //Not most efficient, but for the size of the grid, it's okay
            while(i < game->width * game->height && hovering < 0) {
                if(game->grid[i] != NULL) {
                    if(sprite_is_hovered(game->grid[i]->sprite))
                        hovering = i;
                } else printf("game.c::find_box_hovered: game->grid[%i] is "
                        "NULL\n", i);
                i++;
            }
        } else printf("game.c::find_box_hovered: game->grid is NULL\n");
    } else printf("game.c::find_box_hovered: game is NULL\n");
    return hovering;
}

void update_grid(struct game* game, int box_index) {
    if(game != NULL) {
        if(game->grid != NULL) {
            if(game->grid[box_index] != NULL) {
                if((game->turn-1)/game->players_nb == 0 //Checks if first turn
                        && game->grid[box_index]->player == PLAYER_NONE) {
                    game->grid[box_index]->player = game->player_turn;
                    game->grid[box_index]->points = 3;
                    update_box_sprite(game->grid[box_index]);
                    next_player(game);
                }
                else if(game->grid[box_index]->player == game->player_turn) {
                    game->grid[box_index]->points++;
                    if(game->grid[box_index]->points >= GAME_BOX_MAX_POINTS) {
                        int boxes[1] = {box_index};
                        explode_boxes(game, boxes, 1);
                    }
                    update_box_sprite(game->grid[box_index]);
                    game_render(game);
                    SDL_Delay(300);
                    next_player(game);
                }
            } else printf("game.c:update_grid: game->grid[%i] is NULL\n",
                    box_index);
        } else printf("game.c:update_grid: game->grid is NULL\n");

    } else printf("game.c:update_grid: game is NULL\n");

}

void update_box_sprite(struct box* box) {
    if(box != NULL) {
        if(box->sprite != NULL) {
            if(box->points < 0 || box->points > GAME_BOX_MAX_POINTS)
                printf("game.c::update_box_sprite: Warning! box->points" 
                        " is either smaller than 0 or bigger than the"
                        " max quantity\n");
            if(box->points == 0 || box->player == PLAYER_NONE)
                box->sprite->sprite_pos =
                    PLAYER_NONE * GAME_BOX_SPRITE_NUM_COLS; 
            else
                box->sprite->sprite_pos =
                    box->player*GAME_BOX_SPRITE_NUM_COLS + box->points-1;
        } else printf("game.c::update_box_sprite: box->sprite is NULL\n");
    } else printf("game.c::update_box_sprite: box is NULL\n");
}
void next_player(struct game* game) {
    if(game != NULL) {
        game->turn++;
        game->player_turn = (game->player_turn + 1) % game->players_nb;
    } else printf("game.c::next_player: game is NULL\n");
}

void explode_boxes(struct game* game, int* boxes_indexes, int box_count) {
    if(game != NULL && boxes_indexes != NULL 
            && box_count > 0 && box_count < game->width * game->height) {
        int next_boxes_count = 0;
        int* next_boxes_indexes = malloc(
                sizeof(int) * game->width * game->height
                );
        int i;
        for(i = 0; i < box_count; i++) {
            explode_box(game, 
                    boxes_indexes[i],
                    next_boxes_indexes,
                    &next_boxes_count);
        }
        if(next_boxes_count > 0) {
            game_render(game);
            SDL_Delay(600);
            explode_boxes(game, next_boxes_indexes, next_boxes_count);
        }
    }
}

void explode_box(struct game* game, int box_index, 
        int* next_boxes_indexes, int* next_boxes_count) {
    //Checks box above
    if(box_index > GAME_BOARD_W - 1)
        update_box_from_collision(game, box_index - (game->width), 
                next_boxes_indexes, next_boxes_count);
    //Checks box under
    if(box_index < GAME_BOARD_W * (GAME_BOARD_H-1) - 1)
        update_box_from_collision(game, box_index + (game->width),
                next_boxes_indexes, next_boxes_count);
    //Checks box to the left
    if(box_index % GAME_BOARD_W != 0)
        update_box_from_collision(game, box_index - 1, 
                next_boxes_indexes, next_boxes_count);
    //Checks box to the right
    if(box_index % GAME_BOARD_W != GAME_BOARD_W - 1)
        update_box_from_collision(game, box_index + 1, 
                next_boxes_indexes, next_boxes_count);
    if(game != NULL && game->grid != NULL)
        box_reset(game->grid[box_index]);
}

void update_box_from_collision(struct game* game, int box_index,
        int* next_boxes_indexes, int* next_boxes_count) {
    if(game != NULL && game->grid != NULL) {
        if(next_boxes_indexes != NULL && next_boxes_count != NULL) {
            game->grid[box_index]->points++;
            game->grid[box_index]->player = game->player_turn;
            if(game->grid[box_index]->points >= GAME_BOX_MAX_POINTS) {
                game->grid[box_index]->points = GAME_BOX_MAX_POINTS;
                if(*next_boxes_count < game->width * game->height) {
                    next_boxes_indexes[*next_boxes_count] = box_index;
                    (*next_boxes_count)++;
                } else
                    printf("game.c::update_box_from_collision: box_index"
                            " out of bounds\n");
            }
            update_box_sprite(game->grid[box_index]);
        } else printf("game.c::update_box_from_collision: var related to"
               " next_boxes is NULL\n");
    } else printf("game.c::update_box_from_collision: game or grid is NULL\n");
}
void box_reset(struct box* box) {
    if(box != NULL) {
        box->points = 0;
        box->player = PLAYER_NONE;
        update_box_sprite(box);
    } else printf("game.c::box_reset: box is NULL\n");
}

int game_ended(struct game* game) {
    int i = 0, game_ended = 0;
    if(game != NULL && game->grid != NULL) {
        game_ended = 1;
        enum player player = PLAYER_NONE;
        while(game_ended && i < game->width * game->height) {
            if(game->grid[i] != NULL) {
                if(player == PLAYER_NONE) {
                        player = game->grid[i]->player;
                } else if(game->grid[i]->player != PLAYER_NONE
                        && game->grid[i]->player != player) {
                    game_ended = 0;
                }
            }
            i++;
        }
    }
    return game_ended;    
}

void game_render(struct game* game) {
    if(game != NULL) {
        if(game->renderer != NULL) {
            Uint8 r, g, b;
            game_set_background_color(game, &r, &g, &b);
            SDL_SetRenderDrawColor(game->renderer, r, g, b, 255);
            SDL_RenderClear(game->renderer);
            if(game_render_grid(game) != 1)
                printf("Unable to properly render game grid\n");
            SDL_RenderPresent(game->renderer);
        }
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

















