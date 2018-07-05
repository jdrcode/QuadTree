#include "body.h"
#include "draw.h"
#include <stdlib.h>
#include <stdbool.h>
#include "SDL_window.h"

enum Direction{
    NWdir,
    NEdir,
    SWdir,
    SEdir,
    UNKNOWNdir,
};

struct Quad
{
    const double x, y;
    const double length;
};
typedef struct Quad Quad;

struct BHTree
{
    Body *body;  // body or aggregate body stored in this node
    const Quad quad;   // square region that the tree represents
    struct BHTree *NW; // tree representing northwest quadrant
    struct BHTree *NE; // tree representing northeast quadrant
    struct BHTree *SW; // tree representing southwest quadrant
    struct BHTree *SE; // tree representing southeast quadrant
};
typedef struct BHTree BHTree;

Quad NW(const Quad *curr)
{
    double newx = curr->x + curr->length / 4;
    double newy = curr->y + curr->length / 4;
    double newl = curr->length / 2;
    return (Quad){newx, newy, newl};
}

Quad NE(const Quad *curr)
{
    double newx = curr->x - curr->length / 4;
    double newy = curr->y + curr->length / 4;
    double newl = curr->length / 2;
    return (Quad){newx, newy, newl};
}

Quad SW(const Quad *curr)
{
    double newx = curr->x + curr->length / 4;
    double newy = curr->y - curr->length / 4;
    double newl = curr->length / 2;
    return (Quad){newx, newy, newl};
}

Quad SE(const Quad *curr)
{
    double newx = curr->x - curr->length / 4;
    double newy = curr->y - curr->length / 4;
    double newl = curr->length / 2;
    return (Quad){newx, newy, newl};
}

bool nodes_are_null(const BHTree *current_node)
{
    if(current_node->NW == NULL
    && current_node->NE == NULL
    && current_node->SE == NULL
    && current_node->SW == NULL)
        return true;
    return false;
}

void draw_quad(const Quad *quad, SDL_Renderer * renderer)
{
    draw_square(quad->x,quad->y, quad->length, renderer);
}

bool in_quad(double x, double y, const Quad *quad)
{
    double x_max = quad->x + quad->length/2;
    double x_min = quad->x - quad->length/2;
    double y_max = quad->y + quad->length/2;
    double y_min = quad->y - quad->length/2;
    if(x <= x_max 
    && x >= x_min
    && y <= y_max
    && y >= y_min)
        return true;
    return false;
}

void unknowndirf(double x, double y){
    //printf("rx: %.3e, ry: %.3e\n", x, y );
}

enum Direction determine_quad(double x, double y, const Quad *quad)
{
    Quad checkNW = NW(quad);
    Quad checkNE = NE(quad);
    Quad checkSW = SW(quad);
    Quad checkSE = SE(quad);

    if( in_quad(x,y, &checkNW))
        return NWdir;

    if( in_quad(x,y, &checkNE))
        return NEdir;
    
    if( in_quad(x,y, &checkSW))
        return SWdir;
    
    if( in_quad(x,y, &checkSE))
        return SEdir;
    
    unknowndirf(x,y);
    return UNKNOWNdir;
}

BHTree *bhtree_new(const Quad quad){
    BHTree tmp = {NULL, quad, NULL, NULL, NULL, NULL};
    BHTree *new_bhtree = (BHTree *) malloc(sizeof(BHTree));
    memcpy(new_bhtree, &tmp,sizeof(BHTree));
    return new_bhtree;
}

void center_mass(const Body *new, Body *aggregate){
    double total_mass = new->mass + aggregate->mass;
    double x = (new->rx*new->mass + aggregate->rx*aggregate->mass) / total_mass;    
    double y = (new->ry*new->mass + aggregate->ry*aggregate->mass) / total_mass;
    aggregate->rx = x;
    aggregate->ry = y;    
    aggregate->mass = total_mass;
}

bool usable_agg_body(const BHTree *node, const Body *body){
    const static double delta = 0.5;
    double dx = node->body->rx - body->rx;
    double dy = node->body->ry - body->ry;
    double distance = sqrt(dx*dx + dy*dy);
    double s_d_ratio = node->quad.length / distance;
    if(s_d_ratio <= delta)
        return true;
    return false;
}

void bhtree_add(BHTree *current_node, Body *passed_body){
    Body *pass = NULL;
    bool made_agg = false;

    if(current_node->body == NULL) {
        current_node->body = passed_body;
        return;
    }

    if (nodes_are_null(current_node)){
        pass = current_node->body;
        Body *new_aggregate_body = malloc(sizeof(Body));
        current_node->body = new_aggregate_body;
        Body tmp = {0,0,0,0,0,0,0,0}; // blank body!
        memcpy(current_node->body, &tmp, sizeof(Body));
    }

    enum Direction dir = determine_quad(passed_body->rx, passed_body->ry, &current_node->quad);
    if(dir == NWdir){
        if(current_node->NW == NULL)
            current_node->NW = bhtree_new( NW(&current_node->quad));
        bhtree_add(current_node->NW, passed_body);    
    }
    if(dir == NEdir){
        if(current_node->NE == NULL)
            current_node->NE = bhtree_new( NE(&current_node->quad));
        bhtree_add(current_node->NE, passed_body);    
    }
    if(dir == SWdir){
        if(current_node->SW == NULL)
            current_node->SW = bhtree_new( SW(&current_node->quad));
        bhtree_add(current_node->SW, passed_body);     
    }
    if(dir == SEdir){
        if(current_node->SE == NULL)
            current_node->SE = bhtree_new( SE(&current_node->quad));
        bhtree_add(current_node->SE, passed_body); 
    }
    if(dir == UNKNOWNdir){
        return;
    }

    center_mass(passed_body, current_node->body);

    if(pass != NULL )
        bhtree_add(current_node, pass); 
    
}

void quad_tree_draw(BHTree *this, SDL_Renderer *gRenderer)
{   
    //draw_quad(&this->quad, gRenderer);
    if(nodes_are_null(this))
        draw_body(this->body, gRenderer);
    if(this->NW != NULL)
        quad_tree_draw(this->NW, gRenderer);
    if(this->NE != NULL)
        quad_tree_draw(this->NE, gRenderer);
    if(this->SW != NULL)
        quad_tree_draw(this->SW, gRenderer);
    if(this->SE != NULL)
        quad_tree_draw(this->SE, gRenderer);
}

void quad_update_force(BHTree *current_node, Body *body){
    if(nodes_are_null(current_node) 
    || usable_agg_body(current_node, body)){
        if(current_node->body != body)
            update_force(body, current_node->body);
        return;
    }

    if(current_node->NW != NULL)
        quad_update_force(current_node->NW, body);
    if(current_node->NE != NULL)
        quad_update_force(current_node->NE, body);
    if(current_node->SW != NULL)
        quad_update_force(current_node->SW, body);
    if(current_node->SE != NULL)
        quad_update_force(current_node->SE, body);
}

void clear_bhtree(BHTree *current_node)
{
    if(nodes_are_null(current_node)){
        free(current_node);
        return;
    }
    if(current_node->NW != NULL)
        clear_bhtree(current_node->NW);
    if(current_node->NE != NULL)
        clear_bhtree(current_node->NE);
    if(current_node->SW != NULL)
        clear_bhtree(current_node->SW);
    if(current_node->SE != NULL)
        clear_bhtree(current_node->SE);
    if(nodes_are_null(current_node)){
        free(current_node);
        return;
    }
}

void quad_tree_main(SDL_Renderer *gRenderer, Body bodies[], int body_cnt, double draw_limits)
{
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    BHTree *base = bhtree_new((Quad){0,0,draw_limits});
 

    for(int i = 0; i < body_cnt;i++)
        bhtree_add(base, &bodies[i]);

    quad_tree_draw(base, gRenderer);

    for(int i = 0; i < body_cnt; i++)
        quad_update_force(base, &bodies[i]);
    for(int i = 0; i < body_cnt; i++)
        update_location(&bodies[i], 0.1);

    clear_bhtree(base);

    SDL_RenderPresent(gRenderer);
}

int main(int argc, char *argv[])
{
    if(argc < 2 ){
        printf("specify input!");
    }
    const int SCREEN_WIDTH = 700;
    const int SCREEN_HEIGHT = 700;
    SDL_Window *window = init_window( SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer *gRenderer = make_renderer(window);
 
    double simulation_size = 0;
    int planet_cnt = 0;
    Body *bodies = load_bodies(argv[1], &planet_cnt, &simulation_size);
    draw_set_scale(SCREEN_WIDTH, SCREEN_HEIGHT, simulation_size, simulation_size);
    printf("simsize: %e, bodycnt: %d \n",simulation_size, planet_cnt);
    
    quad_tree_main(gRenderer, bodies, planet_cnt, simulation_size);
    SDL_Delay(10);

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
        quad_tree_main(gRenderer, bodies, planet_cnt, simulation_size);
    }

    close_window(window, gRenderer);
    return 0;
}
