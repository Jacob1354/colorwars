#include <stdio.h>
#include "../include/menu.h"
#include "../include/constants.h"

//General fcts

void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer);

//Menu init fcts
void menu_fill(struct menu* menu, SDL_Renderer* renderer);



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
        menu_gen_sprites(menu, renderer);
        if(menu->sprites != NULL) {
            menu->renderer = renderer;
            menu->players_nb = DEFAULT_PLAYERS_NB;
            menu->bots_nb = DEFAULT_BOTS_NB;
            menu->state = MENU_STATE_NONE;
       }
    }
}

void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer) {
    menu->sprites = calloc(10, sizeof(struct sprite));
}


void menu_run(struct menu* menu);


void menu_delete(struct menu* menu) {
    int i;
    if(menu->sprites != NULL)
        for(i=0; i < menu->sprite_nb; i++)
            free(menu->sprites[i]);
    free(menu);
}
