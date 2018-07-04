#include "brute_force.h"

int counter = 0;
void brute_force_main(SDL_Renderer *gRenderer, Body *bodies, int planet_cnt)
{

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    const double dt = 100;
    for (int i = 0; i < planet_cnt; i++)
    {
        for (int j = 0; j < planet_cnt; j++)
        {
            if (i == j)
                continue;
            update_force( &bodies[i], &bodies[j]);
        }
    }
    if(counter < 10)
    for (int i = 0; i < planet_cnt; i++)
    {
        update_location(&bodies[i], dt);
        draw_body(&bodies[i], gRenderer);
    }
    
    SDL_RenderPresent(gRenderer);
}
