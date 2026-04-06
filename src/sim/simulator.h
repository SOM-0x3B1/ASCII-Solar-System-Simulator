#ifndef ASCII_SSS_SIMULATOR_H
#define ASCII_SSS_SIMULATOR_H

#include <stdlib.h>

#include "../structs.h"
#include "body_array.h"
#include "../graphics/render.h"


/** Performs all of the simulation steps. */
void sim_tick(Simulation *sim);

/** Processes the raw input. */
void sim_process_input(Simulation *sim, Screen *screen, Program *program, Gui *gui, LayerInstances *li);


#endif //ASCII_SSS_SIMULATOR_H