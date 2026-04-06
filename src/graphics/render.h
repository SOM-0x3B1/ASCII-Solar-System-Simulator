#ifndef ASCII_SSS_RENDER_H
#define ASCII_SSS_RENDER_H

#include "../structs.h"
#include "../vector.h"
#include "../sim/simulator.h"


/**
 * Allocates memory for the screenbuffer.
 * @return Successful / error
 */
Error rndr_init(Screen *screen);

/** Frees the memory of the screenbuffer. */
void rndr_dispose(Screen *screen);

/** Resets the FPS counters after disabling fullspeed mode. */
void rndr_reset_fps_measurement(Screen *screen);

/** Prints the content of the layers on the console. */
void rndr_refresh_screen(Program *program, Simulation *sim, Screen *screen, LayerStatic *lp);

/** Updates all layers. */
void rndr_full_render(Program *program, Simulation *sim, Screen *screen, LayerStatic *lp, Gui *gui);


#endif //ASCII_SSS_RENDER_H