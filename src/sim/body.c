#include "body.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../graphics/drawing.h"


#define TRAIL_LENGTH 15


static void swap_body_pointers(Body **a, Body **b) {
    Body *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 *  Collides a lighter body into a heavier one.
 *  The order of parameters is arbitrary.
 */
static void collide(Body *a, Body *b, Simulation *sim) {
    if (b->mass > a->mass)
        swap_body_pointers(&a, &b);
    if (sim->following == b)
        sim->following = a;
    if (sim->editedBody == b)
        sim->editedBody = a;
    a->mass += b->mass;
    a->r = sqrt((a->r * a->r) + (b->r + b->r) * 3.14);
    a->velocity = vec_scalar_divide(
            vec_add(vec_scalar_multiply(a->velocity, a->mass),
                       vec_scalar_multiply(b->velocity, b->mass)),
            a->mass + b->mass);
    barr_remove(&sim->bodyArray, b, sim);
}

/** Draws the info part of a body. */
static void draw_info(Body const *body, LayerInstances *li, Screen *screen) {
    if (li->infoLayer.enabled) {
        Point p = vec_to_point(body->position);
        p.y /= 2;
        p = pnt_subtract(p, screen->offset);
        drw_draw_text(&li->infoLayer, (int) (p.x - strlen(body->name) / 2), (int) p.y, body->name, screen);
    }
}

/** Draws the trail part of a body. */
static void draw_trail(Body const *body, LayerInstances *li, Screen *screen) {
    Trail *t = body->trail.top;
    int i = 1;
    do {
        Point p = t->position;
        p.y /= 2;
        p = pnt_subtract(p, screen->offset);

        char c = i < TRAIL_LENGTH / 1.5 ? '+' : '.';
        i++;

        lyr_write_at_xy(&li->trailLayer, (int) p.x, (int) p.y, c, screen);
        t = t->next;
    } while (t != NULL);
}

/** Draws the body part of a body. */
static void draw_body(Body const *body, Simulation *sim, LayerInstances *li, Screen *screen) {
    Point p = vec_to_point(body->position);
    p.y /= 2;
    p = pnt_subtract(p, screen->offset);

    Point pLight;
    bool drawShadows = !li->infoLayer.enabled && body != sim->sun;
    if (drawShadows) {
        pLight = vec_to_point(body->position);
        Point pLightUnit = vec_to_point(vec_unit_vector(body->position, sim->sun->position));
        pLight = pnt_subtract(pLight, pnt_scalar_multiply(pLightUnit, (int) body->r / 2));
        pLight.y /= 2;
        pLight = pnt_subtract(pLight, screen->offset);
    }

    for (int y = 0; y < screen->height; y++) {
        for (int x = 0; x < screen->width; x++) {
            long long int dX = x - p.x;
            long long int dY = (y - p.y) * 2;
            long long int dx2dy2 = (dX * dX) + (dY * dY);

            long long int dx2dy2Light = 0;
            if (drawShadows) {
                long long int ldX = x - pLight.x;
                long long int ldY = (y - pLight.y) * 2;
                dx2dy2Light = (ldX * ldX) + (ldY * ldY);
            }

            double er = sqrt(body->mass * 100);

            long long int drange = llabs((dx2dy2 / 2) - (long long int) (er * er));

            if (dx2dy2 <= (int) (body->r * body->r)) {
                if (!drawShadows || dx2dy2Light <= (int) (body->r * body->r))
                    lyr_write_at_xy(&li->bodyLayer, x, y, body->color, screen);
                else
                    lyr_write_at_xy(&li->bodyLayer, x, y, ':', screen);
            } else if (li->rangeLayer.enabled && drange < (long long int) (er * 0.8))
                lyr_write_at_xy(&li->rangeLayer, x, y, '.', screen);
        }
    }
    draw_info(body, li, screen);
    draw_trail(body, li, screen);
}

/** Sets the camera position so the followed body is in the center of the screen. */
static void move_camera_to_body(Body *b, Screen *screen, LayerInstances *li) {
    Point p = vec_to_point(b->position);
    p.y /= 2;
    Point screenSize = {screen->width / 2, screen->height / 2};
    p = pnt_subtract(p, screenSize);
    if (li->menuLayer.enabled)
        p.x += 16;
    screen->offset = p;
}


Error bdy_init(Simulation *sim) {
    if (barr_init(&sim->bodyArray) != SUCCESS)
        return ERR_MEMORY; // failed to allocate memory for body array

    sim->sun = bdy_new("Sun", (Vector) {0, 0}, (Vector) {0, 0}, 7, sim->solarMass, '@', sim);
    if (sim->sun == NULL)
        return ERR_MEMORY;

    sim->following = sim->sun;

    return SUCCESS;
}

Body *bdy_new(char *name, Vector pos, Vector v, double r, double mass, char color, Simulation *sim) {
    Body b;
    strcpy(b.name, name);
    b.color = color;
    b.r = r;
    b.mass = mass;
    b.position = pos;
    b.velocity = v;

    b.trail = (TrailQueue) {};
    bdy_trail_queue_init(&b.trail, &b);

    return barr_add(&sim->bodyArray, &b, sim);
}

void bdy_add_g_effect(Body *dest, Body *src, Simulation *sim) {
    double d = vec_distance(dest->position, src->position);
    double d2 = d * d * sim->solarMass;
    double force = src->mass / d2; // TODO: adaptive simulation speed regulation

    Vector v = vec_subtract(dest->position, src->position);
    Vector unitVector = vec_scalar_divide(v, d);
    v = vec_invert(vec_scalar_multiply(unitVector, force));

    dest->velocity = vec_add(dest->velocity, v);
}

void bdy_move(Body *body) {
    body->position = vec_add(body->position, body->velocity); // TODO: adaptive simulation speed regulation
}

void bdy_detectCollision(Body *a, Body *b, Simulation *sim) {
    if (sim->detectCollisionPercentage > 0) {
        double d = vec_distance(a->position, b->position);
        if (d < (a->r + b->r) * sim->detectCollisionPercentage)
            collide(a, b, sim);
    }
}

void bdy_render(LayerInstances *li, Simulation *sim, Screen *screen) {
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        Body *b = &sim->bodyArray.data[i];

        if (sim->following == b)
            move_camera_to_body(b, screen, li);

        draw_body(b, sim, li, screen);
    }
}

void bdy_trail_queue_init(TrailQueue *tq, Body *b) {
    tq->top = (Trail *) malloc(sizeof(Trail));
    tq->top->position = vec_to_point(b->position);
    tq->top->next = NULL;

    tq->capacity = TRAIL_LENGTH;
    tq->length = 1;
}


Trail *bdy_trail_dequeue(TrailQueue *tq) {
    Trail *ct = tq->top;
    for (int i = 0; i < tq->length - 2; ++i)
        ct = ct->next;

    Trail *res = ct->next;
    free(ct->next);
    ct->next = NULL;
    tq->length--;
    return res;
}

void bdy_trail_enqueue(TrailQueue *tq, Vector v) {
    Point p = vec_to_point(v);
    Trail *newT = (Trail *) malloc(sizeof(Trail));
    newT->position = p;
    newT->next = tq->top;
    tq->top = newT;
    tq->length++;

    if (tq->length > tq->capacity)
        bdy_trail_dequeue(tq);
}

void bdy_trail_clear(TrailQueue *tq) {
    Trail *cNode = tq->top;
    while (cNode != NULL) {
        Trail *next = cNode->next;
        free(cNode);
        cNode = next;
    }
}