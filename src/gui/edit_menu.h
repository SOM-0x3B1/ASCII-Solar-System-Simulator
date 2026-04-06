#ifndef ASCII_SSS_EDIT_MENU_H
#define ASCII_SSS_EDIT_MENU_H

#include "../structs.h"
#include "../sim/body.h"
#include "../lib/econio.h"

#define EDIT_MENU_WIDTH 32


/**
 * Set the proper states and display parameters for edit menu.
 * @param key if it's Esc, the cursor jumps to the Exit function
 */
void editm_switch(EconioKey key, Program *program, Gui *gui, Screen *screen, LayerInstances *li);

/** Closes the edit menu and returns to simulation. */
void editm_close(Program *program, Gui *gui, LayerInstances *li, Screen *screen, Simulation *sim);

/** Renders all edit menu components. */
void editm_render(LayerInstances *li, Screen *screen, Gui *gui, Simulation *sim);

/** Reads raw input. */
Error editm_process_input(Program *program, Simulation *sim, Screen *screen, Gui *gui, LayerInstances *li);


#endif //ASCII_SSS_EDIT_MENU_H