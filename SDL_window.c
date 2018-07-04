#include "SDL_window.h"

SDL_Window *init_window(int screen_width, int screen_height)
{
    
    SDL_Window *window;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }

    window = SDL_CreateWindow(
        "QuadTree",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screen_width,
        screen_height,
        SDL_WINDOW_SHOWN);
    

    if(window == NULL)
        printf("WTF NULL WINDOW");

    return window;
}

SDL_Renderer *make_renderer(SDL_Window *window)
{
    SDL_Renderer *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    /*if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }*/
    return gRenderer;
}

void close_window(SDL_Window *window ,SDL_Renderer *gRenderer)
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}
