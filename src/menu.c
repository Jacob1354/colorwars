#include <stdio.h>
#include "../include/menu.h"
#include "../include/constants.h"

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
        menu->sprite_nb = 0;
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
        int i;
        for(i = 0; i < menu->sprite_nb; i++) {
          
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
