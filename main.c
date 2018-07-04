#include <stdio.h>
#include "brute_force.h"
#include <stdbool.h>
#include "SDL_window.h"

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

//////////////////////////////////////
int main(int argc, char *argv[])
{
    SDL_Window *window = init_window( SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer *gRenderer = make_renderer(window);

    double simulation_size = 0;
    int planet_cnt = 0;
    Body *bodies = load_bodies("planets.txt", &planet_cnt, &simulation_size);
    draw_set_scale(SCREEN_WIDTH, SCREEN_HEIGHT, simulation_size, simulation_size);

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        brute_force_main(gRenderer, bodies, planet_cnt);
    }

    close_window(window, gRenderer);
    return 0;
}
