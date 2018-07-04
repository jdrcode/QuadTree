#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>


void draw_filled_circled(double , double , int , SDL_Renderer *);   
void draw_set_scale(int , int , double , double );
double draw_scaled_Y_position(double );
double draw_scaled_X_position(double );
void draw_square(double x, double y, double length, SDL_Renderer *renderer);
double draw_scaled_length(double orginal);


#endif
