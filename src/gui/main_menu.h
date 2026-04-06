#ifndef ASCII_SSS_MAIN_MENU_H
#define ASCII_SSS_MAIN_MENU_H

#include "../structs.h"
#include "../graphics/drawing.h"
#include "../lib/econio.h"


/** Renders the main menu. */
void mmenu_render(Screen *screen, LayerInstances *li, Gui *gui);

/** Closes the main menu and starts the simulation */
void mmenu_start_sim(Program *program, LayerInstances *li);

/** Calls mainMenu_startSim() if ENTER is pressed. */
void mmenu_process_input(Program *program, LayerInstances *li);


#endif //ASCII_SSS_MAIN_MENU_H
