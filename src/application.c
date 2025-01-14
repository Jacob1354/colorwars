#include <stdio.h>
#include "../include/application.h"

//App init fcts
struct application* init_app(struct application* app);




struct application* application_initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        printf("Unable to initialize sdl");
        return NULL;
    }
    struct application* app = malloc(sizeof(struct application));
    if (app == NULL) {
        printf(ERR_MSG_ALLOC, "struct application");
        return NULL;
    }
    return init_app(app);
}

struct application* init_app(struct application* app) {
    app->state = APPLICATION_STATE_MENU;
    app->window = SDL_CreateWindow("app_wnd", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WND_W, WND_H, 0); 
    if(app->window == NULL) {
        printf(SDL_GetError());
        return NULL;
    }
    app->renderer = SDL_CreateRenderer(app->window, -1, 0);
    app->menu = menu_init(app->renderer);
    if(app->menu == NULL) {
        printf(ERR_MSG_ALLOC, "app menu");
        return NULL;
    }
    app->game = NULL;
    return app;
}

void application_run(struct application *app) {
    while(app->state != APPLICATION_STATE_QUIT) {
        switch(app->state) {
            case APPLICATION_STATE_MENU:
                if(app->menu != NULL) {
                    menu_run(app->menu); 
                    if(app->menu->state == MENU_STATE_QUIT)
                        app->state = APPLICATION_STATE_QUIT;
                    else if(app->menu->state == MENU_STATE_PLAY)
                        app->state = APPLICATION_STATE_PLAY;
                    menu_reset(app->menu);
                }
                break;
            case APPLICATION_STATE_PLAY:
                app->game = game_create(GAME_BOARD_W, GAME_BOARD_H,
                        app->menu->players_nb,
                        app->menu->bots_nb,
                        app->renderer);
                if(app->game != NULL) {
                    game_run(app->game);
                    if(app->game->state == GAME_STATE_FORCE_QUIT)
                        app->state = APPLICATION_STATE_QUIT;
                    else if(app->game->state == GAME_STATE_QUIT)
                        app->state = APPLICATION_STATE_MENU;
                }
                game_delete(app->game);
                break;
            case APPLICATION_STATE_QUIT:
            default:
        }
    }
}


void application_shut_down(struct application *app) {
    menu_delete(app->menu);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    IMG_Quit();
    SDL_Quit();
}





















