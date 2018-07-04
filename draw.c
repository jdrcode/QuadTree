#include "draw.h"

double windowX = 1;
double windowY = 1;
double max_X = 1;
double max_Y = 1;

void draw_square(double x, double y, double length, SDL_Renderer *renderer)
{
    x = draw_scaled_X_position(x);
    y = draw_scaled_Y_position(y);
    length = draw_scaled_length(length);
    double left = x - length / 2;
    double right =  x + length / 2;
    double top = y + length / 2;
    double bot = y - length / 2;
    //top
    //printf("x: %e, y: %e, length: %e\n", x, y, length);
   // printf("left: %e, top: %e\n", left, top);
    SDL_RenderDrawLine(renderer, left, top, right, top);
    //left
    SDL_RenderDrawLine(renderer, left, top, left, bot);
    //right
    SDL_RenderDrawLine(renderer, right, top, right, bot);
    //bot
    SDL_RenderDrawLine(renderer, left, bot, right, bot);
}

void draw_filled_circled(double xIn, double yIn, int radius, SDL_Renderer *renderer)
{
    int x0 = draw_scaled_X_position(xIn);
    int y0 = draw_scaled_Y_position(yIn);
    int x = radius;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius - 1 << 1);

    while (x >= y)
    {
        SDL_RenderDrawLine(renderer, x0 + x, y0 + y, x0 - x, y0 + y);
        SDL_RenderDrawLine(renderer, x0 + y, y0 + x, x0 - y, y0 + x);
        SDL_RenderDrawLine(renderer, x0 - x, y0 - y, x0 + x, y0 - y);
        SDL_RenderDrawLine(renderer, x0 - y, y0 - x, x0 + y, y0 - x);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void draw_set_scale(int windowXi, int windowYi, double max_Xi, double max_Yi)
{
    windowX = windowXi;
    windowY = windowYi;
    max_X = max_Xi;
    max_Y = max_Yi;
}

double draw_scaled_Y_position(double orginal)
{
    return (orginal / max_Y) * windowY + windowY / 2.0;
}

double draw_scaled_X_position(double orginal)
{
    return (orginal / max_X) * windowX + windowX / 2.0;
}

double draw_scaled_length(double orginal)
{
    return ((orginal / max_X) * windowX);
}
