#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#include "../structs.h"
#include "simulator.h"
#include "body_array.h"
#include "../lib/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"


/** Allocates the body array, and creates a sun.*/
Error bdy_init(Simulation *sim);

/**
 * Creates a new body.
 * @param v velocity
 * @param r radius
 * @param mass relative to Earth's mass (1: Earth's mass)
 * @return The pointer to the new body
 */
Body *bdy_new(char *name, Vector pos, Vector v, double r, double mass, char color, Simulation *sim);

/** Adds the gravitational effect of the src body to the destination body. */
void bdy_add_g_effect(Body *dest, Body *src, Simulation *sim);

/** Move a body by its velocity. */
void bdy_move(Body *body);

/** Detect if two bodies collide. */
void bdy_detectCollision(Body *a, Body *b, Simulation *sim);

/** Renders all bodies. */
void bdy_render(LayerInstances *li, Simulation *sim, Screen *screen);


/** Initializes a trail queue for a body. */
void bdy_trail_queue_init(TrailQueue *tq, Body *b);

/** Adds a new trail point to a trail queue. */
void bdy_trail_enqueue(TrailQueue *tq, Vector v);

/** Clears and frees a trail queue. */
void bdy_trail_clear(TrailQueue *tq);


#endif //ASCII_SSS_BODY_H