#ifndef ASCII_SSS_BODY_EDITOR_H
#define ASCII_SSS_BODY_EDITOR_H

#include "../structs.h"
#include "../sim/body.h"


/** Set the proper states to read text input for the body editor. */
void bedit_switch(Program *p);

/** Renders the current body editor Gui element.*/
void bedit_render(Program *program, LayerInstances *li, Screen *screen, Gui *gui);

/** Processes the input text after pressing enter.
 *  @return Success / error
 */
Error bedit_process_text_input(Program *program, Gui *gui, Simulation *sim);

/** Moves and places the edited body on the simulation window. */
void bedit_process_placement_input(Program *program, Gui *gui, Simulation *sim);


#endif //ASCII_SSS_BODY_EDITOR_H
