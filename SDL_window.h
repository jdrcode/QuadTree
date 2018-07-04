#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <SDL2/SDL.h>

SDL_Window *init_window(int screen_width, int screen_height);
SDL_Renderer *make_renderer(SDL_Window *window);
void close_window(SDL_Window *window ,SDL_Renderer *gRenderer);

#endif