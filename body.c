#include "body.h"

Body make_body(double irx, double iry, double ivx, double ivy, double imass, int isize){
  Body pass;
  pass.rx = irx;
  pass.ry = iry;
  pass.vx = ivx;
  pass.vy = ivy;
  pass.mass = imass;
  pass.size = isize;
  return pass;
}

void update_force(Body *a, const Body *b)
{
  const double G = 6.67e-11; // gravational constant
  const double EPS = 3E4;    // softening parameter+ EPS * EPS
  double dx = b->rx - a->rx;
  double dy = b->ry - a->ry;
  double dist = sqrt(dx * dx + dy * dy);
  double F = G * a->mass * b->mass / (dist * dist + EPS * EPS);
  a->fx += F * dx / dist;
  a->fy += F * dy / dist;
}

void update_location(Body *a, double dt)
{
  a->vx += dt * (a->fx / a->mass);
  a->vy += dt * (a->fy / a->mass);
  a->rx += dt * a->vx;
  a->ry += dt * a->vy;
  a->fx = 0;
  a->fy = 0;
}

void draw_body(const Body *a, SDL_Renderer *gRenderer)
{
  draw_filled_circled(a->rx, a->ry, a->size, gRenderer);
}

Body *load_bodies(char* csvFile, int *planet_cnt_passback, double *simulation_size_passback){
    FILE* stream = fopen(csvFile, "r");
    char line[1024];
    int planet_cnt = atoi(fgets(line, 1024,stream));
    double resize_ratio = 2 * atof(fgets(line, 1024,stream));
    Body *bodies = malloc(sizeof(Body)  * planet_cnt);
    *planet_cnt_passback = planet_cnt;
    *simulation_size_passback = resize_ratio;

    for( int q = 0; q < planet_cnt; q++) {
        fgets(line, 1024, stream);
        static const char delimiter[] = " ";
        
        char* token = strtok(line, delimiter);
        char *hold[6];
        int i = 0;
        while(token != NULL){
            //printf("%s \n", token );
            hold[i++] = token;
            token = strtok(NULL, delimiter);
        }
        bodies[q].rx = atof(hold[0]);
        bodies[q].ry = atof(hold[1]);
        bodies[q].vx = atof(hold[2]);
        bodies[q].vy = atof(hold[3]);
        bodies[q].mass = atof(hold[4]);
        bodies[q].size = 4;
    }
    fclose(stream);
    return bodies;
}