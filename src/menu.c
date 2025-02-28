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

//Here's the order for the coordinates :
//bg, title, play, quit, players_txt, bots_txt, up_arrow_player, up_arrow_bot,
//down_arrow_player, down_arrow_bot, digits_player, digits_bot 
const int MENU_SPRITES_X[MENU_SPRITE_QTT] = { 
    0, 28, 216, 417, 160, 193, 333, 333, 333, 333, 408, 408
};

const int MENU_SPRITES_Y[MENU_SPRITE_QTT] = { 
    1, 152, 319, 315, 427, 540, 450, 561, 490, 601, 482, 593
};


//Data structs

enum sprite_id {
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
//=================================================================
//Generates the menu sprites
void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer);
//Retrunrs the number of columns for a sprite ID (hard coded values)
int find_sprite_cols(enum sprite_id sprite);
//Retuns true if the sprite needs a new sprite sheet
int need_new_spritesheet(enum sprite_id sprite);
//Returns the path index of MENU_SPRITES_PATHS for the sprite id
int path_nb(enum sprite_id sprite);
//Returns the position of the sprite relative to the spritesheet (hard coded values)
int get_sprite_pos(struct menu* menu, enum sprite_id id);

//Menu init fcts
//=================================================================
void menu_fill(struct menu* menu, SDL_Renderer* renderer);

//Menu run fcts
//=================================================================
void menu_event_loop(struct menu* menu, SDL_Event e);
//Handles mouse clicks
void menu_mouse_click(struct menu* menu);
void menu_change_players_nb(struct menu* menu, int increment);
void menu_change_bots_nb(struct menu* menu, int increment);
void menu_render(struct menu* menu);
//Returns true if there has been a state change
int update_menu_focus_state(struct menu* menu);
void update_sprites(struct menu* menu);
//Sets the buttons sprite to their default position
void set_buttons_to_default(struct menu* menu);


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
            menu->renderer = renderer;
            menu->players_nb = DEFAULT_PLAYERS_NB;
            menu->bots_nb = DEFAULT_BOTS_NB;
            menu->state = MENU_STATE_NONE;
            menu_gen_sprites(menu, renderer);
       }
    }
}

void menu_gen_sprites(struct menu* menu, SDL_Renderer* renderer) {
    if(menu != NULL && menu->sprites != NULL) {
        unsigned int dims[2];
        int i;
        struct spritesheet* ss;
        for(i = 0; i < menu->sprite_nb; i++) {
            int sprite_cols = find_sprite_cols(i);
            int sprite_rows = 1;
            if(need_new_spritesheet(i))
                ss = spritesheet_create(MENU_SPRITES_PATHS[path_nb(i)],
                        sprite_rows, sprite_cols, sprite_cols * sprite_rows,
                        renderer);
            get_png_dimensions(MENU_SPRITES_PATHS[path_nb(i)], dims);
            menu->sprites[i] = sprite_create(ss, get_sprite_pos(menu, i), MENU_SPRITES_X[i], 
                    MENU_SPRITES_Y[i], dims[0], dims[1]);
       }
    }
}


int find_sprite_cols(enum sprite_id id) {
    int id_qtt = 1;
    if(id == SPRITE_PLAY || id == SPRITE_QUIT ||
            (id >= SPRITE_PLAYERS_ARROW_UP
             && id <= SPRITE_BOTS_ARROW_DOWN))
        id_qtt = 2;
    else if(id == SPRITE_PLAYERS_DIGIT
            || id == SPRITE_BOTS_DIGIT)
        id_qtt = 6;
    return id_qtt;
        
}

int need_new_spritesheet(enum sprite_id id) {
        return !(id == SPRITE_BOTS_ARROW_UP
                || id == SPRITE_BOTS_ARROW_DOWN
                || id == SPRITE_BOTS_DIGIT);
}

int path_nb(enum sprite_id id) {
    int path_nb = id;
    if (id == SPRITE_BOTS_ARROW_UP
           || id == SPRITE_PLAYER_ARROW_DOWN)
       path_nb -= 1; 
    else if (id == SPRITE_BOTS_ARROW_DOWN
            || id == SPRITE_PLAYERS_DIGIT)
        path_nb -= 2;
    else if (id == SPRITE_BOTS_DIGIT)
        path_nb -= 3;
    return path_nb;
}

int get_sprite_pos(struct menu* menu, enum sprite_id id) {
    int sprite_pos = 0;
    if(id == SPRITE_PLAYERS_DIGIT)
        sprite_pos = menu->players_nb;
    else if (id == SPRITE_BOTS_DIGIT)
        sprite_pos = menu->bots_nb;
    return sprite_pos;
}

//Menu run
void menu_run(struct menu* menu) {
    if(menu != NULL) {
        SDL_Event e;
        while(menu->state != MENU_STATE_QUIT
                && menu->state != MENU_STATE_PLAY) {
            menu_event_loop(menu, e); 
            if(menu->state != MENU_STATE_QUIT && menu->state != MENU_STATE_PLAY)
                if(update_menu_focus_state(menu))
                    update_sprites(menu);
            menu_render(menu);
        }
    }
}

void menu_event_loop(struct menu* menu, SDL_Event e) {
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT)
              menu->state = MENU_STATE_QUIT;
         else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
             menu_mouse_click(menu);
    }
}

void menu_mouse_click(struct menu* menu) {
    if(menu != NULL) {
        switch(menu->state) {
            case MENU_STATE_PLAY_FOCUS:
                menu->state = MENU_STATE_PLAY;
                break;
            case MENU_STATE_QUIT_FOCUS:
                menu->state = MENU_STATE_QUIT;
                break;
            case MENU_STATE_PLAYERS_UP_FOCUS:
                menu_change_players_nb(menu, 1);
                break;
            case MENU_STATE_PLAYERS_DOWN_FOCUS:
                menu_change_players_nb(menu, -1);
                break;
            case MENU_STATE_BOTS_UP_FOCUS:
                menu_change_bots_nb(menu, 1);
                break;
            case MENU_STATE_BOTS_DOWN_FOCUS:
                menu_change_bots_nb(menu, -1);
                break;
            default:
        } 
    }
}

void menu_change_players_nb(struct menu* menu, int increment) {
    menu->players_nb = (menu->players_nb+increment);
    if(menu->players_nb < MIN_PLAYERS) menu->players_nb = MAX_PLAYERS;
    if(menu->players_nb > MAX_PLAYERS) menu->players_nb = MIN_PLAYERS;
    menu->sprites[SPRITE_PLAYERS_DIGIT]->sprite_pos = menu->players_nb;
}

void menu_change_bots_nb(struct menu* menu, int increment) {
    menu->bots_nb = (menu->bots_nb+increment)%(MAX_BOTS+1);
    if(menu->bots_nb < MIN_BOTS) menu->bots_nb = MAX_BOTS;
    menu->sprites[SPRITE_BOTS_DIGIT]->sprite_pos = menu->bots_nb;
}

void menu_render(struct menu* menu) {
    if(menu != NULL) {
        int i;
        for(i = 0; i < menu->sprite_nb; i++)
            sprite_render(menu->sprites[i]);
        SDL_RenderPresent(menu->renderer);
    }
}

int update_menu_focus_state(struct menu* menu) {
    if(menu != NULL) {
        enum menu_state old_state = menu->state;
        if(sprite_is_hovered(menu->sprites[SPRITE_PLAY]))
            menu->state = MENU_STATE_PLAY_FOCUS;
        else if(sprite_is_hovered(menu->sprites[SPRITE_QUIT]))
            menu->state = MENU_STATE_QUIT_FOCUS;
        else if(sprite_is_hovered(menu->sprites[SPRITE_PLAYERS_ARROW_UP]))
            menu->state = MENU_STATE_PLAYERS_UP_FOCUS;
        else if(sprite_is_hovered(menu->sprites[SPRITE_PLAYER_ARROW_DOWN]))
            menu->state = MENU_STATE_PLAYERS_DOWN_FOCUS;
        else if(sprite_is_hovered(menu->sprites[SPRITE_BOTS_ARROW_UP]))
            menu->state = MENU_STATE_BOTS_UP_FOCUS;
        else if(sprite_is_hovered(menu->sprites[SPRITE_BOTS_ARROW_DOWN]))
            menu->state = MENU_STATE_BOTS_DOWN_FOCUS;
        else menu->state = MENU_STATE_NONE;
        return old_state != menu->state;
    } else return -1;
}

void update_sprites(struct menu* menu) {
    if(menu != NULL) {
        set_buttons_to_default(menu);
        switch(menu->state) {
            case MENU_STATE_PLAY_FOCUS:
                menu->sprites[SPRITE_PLAY]->sprite_pos = 1; 
                break;
            case MENU_STATE_QUIT_FOCUS:
                menu->sprites[SPRITE_QUIT]->sprite_pos = 1; 
                break;
            case MENU_STATE_PLAYERS_UP_FOCUS:
                menu->sprites[SPRITE_PLAYERS_ARROW_UP]->sprite_pos = 1; 
                break;
            case MENU_STATE_PLAYERS_DOWN_FOCUS:
                menu->sprites[SPRITE_PLAYER_ARROW_DOWN]->sprite_pos = 1; 
                break;
            case MENU_STATE_BOTS_UP_FOCUS:
                menu->sprites[SPRITE_BOTS_ARROW_UP]->sprite_pos = 1; 
                break;
            case MENU_STATE_BOTS_DOWN_FOCUS:
                menu->sprites[SPRITE_BOTS_ARROW_DOWN]->sprite_pos = 1; 
                break;
            default:
        }
    }
}

void set_buttons_to_default(struct menu* menu) {
    if(menu != NULL) {
        menu->sprites[SPRITE_PLAY]->sprite_pos = 0;
        menu->sprites[SPRITE_QUIT]->sprite_pos = 0;
        menu->sprites[SPRITE_PLAYERS_ARROW_UP]->sprite_pos = 0;
        menu->sprites[SPRITE_PLAYER_ARROW_DOWN]->sprite_pos = 0;
        menu->sprites[SPRITE_BOTS_ARROW_UP]->sprite_pos = 0;
        menu->sprites[SPRITE_BOTS_ARROW_DOWN]->sprite_pos = 0;
    }
}

//Menu delete
void menu_delete(struct menu* menu) {
    int i;
    if(menu->sprites != NULL)
        for(i=0; i < menu->sprite_nb; i++)
            if(menu->sprites[i] != NULL 
                    && menu->sprites[i]->spritesheet != NULL) {
                //After SPRITE_NB_BOTS are pairs of sprites using the same
                //spritesheet. Therefore, must only free one out of 2
                if(i < SPRITE_NB_BOTS || i%2 == 0)
                    spritesheet_delete(menu->sprites[i]->spritesheet);
                free(menu->sprites[i]);
            }
    free(menu);
}

//Menu reset
int menu_reset(struct menu* menu) {
    if(menu == NULL) {
        printf("Cannot reset NULL menu\n");
        return 0;
    }
    menu->state = MENU_STATE_NONE;
    int i;
    for(i = 0; i < menu->sprite_nb; i++)
        if(menu->sprites[i] != NULL)
            if(i != SPRITE_PLAYERS_DIGIT 
                    && i != SPRITE_BOTS_DIGIT)
                menu->sprites[i]->sprite_pos = 0;
    return 1;
}


















