#ifndef ASCII_SSS_FS_H
#define ASCII_SSS_FS_H

#include "structs.h"
#include "sim/simulator.h"
#include "graphics/render.h"
#include "graphics/layer.h"


/** Loads the settings from setting.ini.
 *  @return Success / error
 */
Error fs_load_settings(Simulation *sim, Screen *screen);

/** Loads the earth animation and title for the main menu.
 *  @return Success / error
 */
Error fs_load_main_menu(Gui *gui);

/** Sets the proper program states to read input text for export. */
void fs_export_switch(Program *program);

/** Renders the export prompt. */
void fs_export_render(Gui *gui, LayerInstances *li, Screen *screen);

/** Sets the proper program states to read input text for import. */
void fs_import_switch(Program *program);

/** Renders the import prompt. */
void fs_import_render(Gui *gui, LayerInstances *li, Screen *screen);

/** Reads the name of the export file. */
Error fs_saving_process_text_input(Gui *gui, Program *program, Simulation *sim, Screen *screen, LayerInstances *li);


#endif //ASCII_SSS_FS_H
