#ifndef ASCII_SSS_OVERLAY_H
#define ASCII_SSS_OVERLAY_H

#include "../structs.h"
#include "../graphics/drawing.h"
#include "../sim/body.h"


/** Renders the overlay. */
void ovl_render(Program *program, Simulation *sim, Screen *screen, LayerInstances *li);


#endif //ASCII_SSS_OVERLAY_H