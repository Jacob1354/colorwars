#include "spritesheet.h"

SDL_Surface* surface_load(const char* filename); 
SDL_Texture* texture_load(SDL_Surface* surface, SDL_Renderer* renderer, const char* filename);
int spritesheet_fill(struct spritesheet* spritesheet, 
                        int num_rows,
                        int num_columns,
                        int num_sprites,
                        int width,
                        int height,
                        float scale,
                        SDL_Texture* texture,
                        SDL_Renderer* renderer);


struct spritesheet *spritesheet_create(const char *filename,
                                       int num_rows,
                                       int num_columns,
                                       int num_sprites,
                                       SDL_Renderer* renderer) {
	struct spritesheet *spritesheet;
    spritesheet = (struct spritesheet*) malloc(sizeof(struct spritesheet));
    SDL_Surface* loaded_surface = NULL;
    if(spritesheet != NULL) {
          loaded_surface = surface_load(filename);
          SDL_Texture* loaded_texture = NULL;
          if(loaded_surface != NULL) loaded_texture = texture_load(loaded_surface, renderer, filename);
          if(loaded_texture != NULL) {
              if(spritesheet_fill(spritesheet, num_rows, num_columns, num_sprites, 
                          loaded_surface->w / num_columns,  loaded_surface->h / num_rows,
                          1.0, loaded_texture, renderer) == -1)
                  printf("Unable to allocate memory for sprite from %s.", filename);
          }
    }
    SDL_FreeSurface(loaded_surface);
    return spritesheet;
}

/*
 * Simply fills the spritesheet passed as param with the params given.
 * The user need to make sure the pointers passed for the elements of
 * the spritesheet are valid.
 *
 * @return int returns -1 if the spritesheet pointer is NULL and 1 otherwise 
*/
int spritesheet_fill(struct spritesheet* spritesheet, 
                        int num_rows,
                        int num_columns,
                        int num_sprites,
                        int width,
                        int height,
                        float scale,
                        SDL_Texture* texture,
                        SDL_Renderer* renderer
                    ) {
    if(spritesheet == NULL) return -1;
    else {
        spritesheet->num_rows = num_rows;
        spritesheet->num_columns = num_columns;
        spritesheet->num_sprites = num_sprites;
        spritesheet->sprite_width = width;
        spritesheet->sprite_height = height;
        spritesheet->scale = scale;
        spritesheet->texture = texture;
        spritesheet->renderer = renderer;
    }
    return 1;
}


/*
 * Loads and returns pointer to surface, from file path, or NULL if error occured
*/
SDL_Surface* surface_load(const char* filename) {
    SDL_Surface *loaded_surface = IMG_Load(filename);
	if (loaded_surface == NULL) {
        printf("Unable to load image %s: %s\n",
                filename, IMG_GetError());
	}
    return loaded_surface;
}

/*
 * Loads and returns pointer to texture, from surface, or NULL if error occured
*/
SDL_Texture* texture_load(SDL_Surface* surface, SDL_Renderer* renderer, const char* filename) {
    SDL_Texture* loaded_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (loaded_texture == NULL) {
	    printf("Unable to create texture from %s: %s\n",
                filename, SDL_GetError());
    }
    return loaded_texture;
}

void spritesheet_delete(struct spritesheet *spritesheet) {
    SDL_DestroyTexture(spritesheet->texture);
    free(spritesheet);
}

void spritesheet_render(struct spritesheet *spritesheet,
                        int x, int y, int alpha, int sprite) {
    int srcx = spritesheet->sprite_width * (sprite % spritesheet->num_columns);
    int srcy = spritesheet->sprite_height * (sprite / spritesheet->num_columns);
    SDL_Rect srcrect = {srcx, srcy, spritesheet->sprite_width, spritesheet->sprite_height};
    SDL_Rect dstrect = {x, y, (int)(spritesheet->scale * spritesheet->sprite_width),
                              (int)(spritesheet->scale * spritesheet->sprite_height)};
    SDL_SetTextureAlphaMod(spritesheet->texture, alpha);
	SDL_RenderCopy(spritesheet->renderer, spritesheet->texture, &srcrect, &dstrect);
}
