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
    app->menu = NULL;/*menu_init();
    if(app->menu == NULL) {
        printf(ERR_MSG_ALLOC, "app menu");
        return NULL;
    }*/
    app->game = NULL;
}

void application_run(struct application *app) {
    SDL_Delay(5000);
}


void application_shut_down(struct application *app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    IMG_Quit();
    SDL_Quit();
}




















