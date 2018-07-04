#ifndef BODY_H
#define BODY_H

#include <math.h>
#include "draw.h"

typedef struct {
  double rx, ry;
  double vx, vy;
  double fx, fy;
  double mass;
  int size;
} Body;

Body make_body(double irx, double iry, double ivx, double ivy, double imass, int isize);
void update_force(Body *a, const Body *b);
void update_location(Body *a, double dt);
void draw_body(const Body *a,SDL_Renderer *gRenderer);
Body *load_bodies(char* csvFile, int *planet_cnt_passback, double *simulation_size_passback);

#endif /*BODY_H*/
