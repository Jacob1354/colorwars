#include <stdio.h>
#include "../include/menu.h"
#include "../include/spritesheet.h"
#include "../include/constants.h"
#include "../include/img_reader.h"

//const vars
const char MENU_SPRITES_PATHS[9][100] = { 
    "./assets/menu/background.png",
    "./assets/menu/title.png", 
    "./assets/menu/play.png",
    "./assets/menu/quit.png", 
    "./assets/menu/players.png",
    "./assets/menu/bots.png", 
    "./assets/menu/arrowup.png",
    "./assets/menu/arrowdown.png", 
    "./assets/menu/digits.png" 
};

const int MENU_SPRITES_X[MENU_SPRITE_QTT] = { 
    0, 28, 216, 417, 160, 193, 333, 333, 333, 333, 408, 408
};

const int MENU_SPRITES_Y[MENU_SPRITE_QTT] = { 
    1, 152, 319, 315, 427, 540, 482, 593, 458, 569, 482, 593
};


//Data structs

enum sprite_pos {
    SPRITE_BACKGROUND = 0,
    SPRITE_TITLE,
    SPRITE_PLAY,
    SPRITE_QUIT,
    SPRITE_NB_PLAYERS,
    SPRITE_NB_BOTS,
    SPRITE_PLAYERS_ARROW_UP,
    SPRITE_BOTS_ARROW_UP,
    SPRITE_PLAYER_ARROW_DOWN,
    SPRITE_BOTS_ARROW_DOWN,
    SPRITE_PLAYERS_DIGIT,
    SPRITE_BOTS_DIGIT
};


//General fcts
void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer);

//Menu init fcts
void menu_fill(struct menu* menu, SDL_Renderer* renderer);

//Menu run fcts
void menu_render(struct menu* menu);




struct menu* menu_init(SDL_Renderer* renderer) {
    struct menu* menu = malloc(sizeof(struct menu));
    if(menu == NULL) printf(ERR_MSG_ALLOC, "menu");
    else {
        if (renderer != NULL)
            menu_fill(menu, renderer);
        else {
            printf("Error : menu's renderer was NULL\n");
            menu = NULL;
        }
    }
    return menu;
}

void menu_fill(struct menu* menu, SDL_Renderer* renderer) {
    if(menu != NULL && renderer != NULL) {
        menu->sprite_nb = MENU_SPRITE_QTT;
        menu->sprites = calloc(MENU_SPRITE_QTT, sizeof(struct sprite));
        if(menu->sprites != NULL) {
            menu_gen_sprites(menu, renderer);
            menu->renderer = renderer;
            menu->players_nb = DEFAULT_PLAYERS_NB;
            menu->bots_nb = DEFAULT_BOTS_NB;
            menu->state = MENU_STATE_NONE;
       }
    }
}

void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer) {
    if(menu != NULL && menu->sprites != NULL) {
        unsigned int dims[2];
        int i;
        struct spritesheet* ss;
        for(i = 0; i < menu->sprite_nb; i++) {
            if(i == SPRITE_PLAYERS_DIGIT) {
                ss = spritesheet_create(MENU_SPRITES_PATHS[i-2], 1, 5, 5, renderer);
                get_png_dimensions(MENU_SPRITES_PATHS[i-2], dims);
            } else if(i == SPRITE_BOTS_DIGIT) {
                get_png_dimensions(MENU_SPRITES_PATHS[i-3], dims);
            }
            else if(i ==  SPRITE_BOTS_ARROW_UP)
                get_png_dimensions(MENU_SPRITES_PATHS[i-1], dims);
            else if(i == SPRITE_PLAYER_ARROW_DOWN) {
                ss = spritesheet_create(MENU_SPRITES_PATHS[i-1], 1, 1, 1, renderer);
                get_png_dimensions(MENU_SPRITES_PATHS[i-1], dims);
            }
            else if(i == SPRITE_BOTS_ARROW_DOWN) {
                get_png_dimensions(MENU_SPRITES_PATHS[i-2], dims);
            }
            else {
                ss = spritesheet_create(MENU_SPRITES_PATHS[i], 1, 1, 1, renderer);
                get_png_dimensions(MENU_SPRITES_PATHS[i], dims);
            }
            menu->sprites[i] = sprite_create(ss, 0, MENU_SPRITES_X[i], 
                    MENU_SPRITES_Y[i], dims[0], dims[1]);
        }
    }
}

//Menu run
void menu_run(struct menu* menu) {
    SDL_Event e;
    while(menu->state != MENU_STATE_QUIT) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT)
                menu->state = MENU_STATE_QUIT;
        }
        menu_render(menu);
    }
}


void menu_render(struct menu* menu) {
    int i;
    for(i = 0; i < menu->sprite_nb; i++)
        sprite_render(menu->sprites[i]);
    SDL_RenderPresent(menu->renderer);
}


//Menu delete
void menu_delete(struct menu* menu) {
    int i;
    if(menu->sprites != NULL)
        for(i=0; i < menu->sprite_nb; i++)
            free(menu->sprites[i]);
    free(menu);
}
