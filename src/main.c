#include <stdio.h>
#include <stdbool.h>

#include "structs.h"
#include "fs.h"
#include "graphics/render.h"
#include "graphics/layer.h"
#include "sim/simulator.h"
#include "sim/body.h"
#include "sim/body_array.h"
#include "gui/edit_menu.h"
#include "gui/body_editor.h"
#include "gui/main_menu.h"
#include "gui/error.h"
#include "lib/econio.h"


/** Initialize modules with dynamic memory management. */
static void init_modules(Program *program, LayerStatic *ls, Simulation *sim, Screen *screen);

/** Free allocated memory, and clear the screen. */
static void exit_program(LayerStatic *ls, Simulation sim, Screen *screen);


int main() {
    econio_set_title("ASCII Solar System Simulator");
    econio_rawmode();

    // Default settings

    Program program;
    program.sleepTime = 0.02;
    program.error = SUCCESS;
    program.exiting = false;
    program.state = PROGRAM_STATE_MAIN_MENU;

    Screen screen;
    screen.width = 119;
    screen.height = 30;
    screen.offset = (Point) {0, 0};
    screen.fps = 0;
    screen.targetFPS = 30;
    screen.frameCount = 0; // Frames since last reset (~1s)

    Simulation sim;
    sim.solarMass = 333000;
    sim.detectCollisionPercentage = 0.8;
    sim.fullSpeed = false;
    sim.pausedByUser = false;
    sim.trailSpacingCounter = 0;

    Gui gui;
    gui.editMenu_state = EDIT_MENU_STATE_MAIN;
    gui.cursorPos = 0;
    gui.mainMenu_animation_frameIndex = 0;

    LayerStatic layerStatic;


    // Attept to load settings.ini

    program.error = fs_load_settings(&sim, &screen);

    screen.bufferSize = screen.height * screen.width * sizeof(char);

    init_modules(&program, &layerStatic, &sim, &screen);


    // Attempt to load main menu earth animation and title
    if (fs_load_main_menu(&gui) != SUCCESS) {
        mmenu_start_sim(&program, &layerStatic.layerInstances);
        err_render(ERR_MAIN_MENU_ART_MISSING, &screen, &layerStatic.layerInstances);
        rndr_refresh_screen(&program, &sim, &screen, &layerStatic);
        err_await_confirmation();
    }


    // Adds some default bodies to the sim
    bdy_new("Mars", (Vector) {-50, 0}, (Vector) {0, -0.15}, 3, 0.15, '#', &sim);
    bdy_new("Venus", (Vector) {13, 0}, (Vector) {0, 0.3}, 4, 0.815, '#', &sim);
    bdy_new("Earth", (Vector) {30, 0}, (Vector) {0, 0.2}, 5, 1, '#', &sim);


    // Main program loop
    while (!program.exiting) {
        if (program.error == SUCCESS) {
            switch (program.state) {
                case PROGRAM_STATE_MAIN_MENU:
                    mmenu_render(&screen, &layerStatic.layerInstances, &gui);
                    mmenu_process_input(&program, &layerStatic.layerInstances);
                    rndr_refresh_screen(&program, &sim, &screen, &layerStatic);
                    break;
                case PROGRAM_STATE_SIMULATION:
                    sim_tick(&sim);
                    sim_process_input(&sim, &screen, &program, &gui, &layerStatic.layerInstances);
                    break;
                case PROGRAM_STATE_EDIT_MENU:
                    sim_tick(&sim);
                    program.error = editm_process_input(&program, &sim, &screen, &gui, &layerStatic.layerInstances);
                    break;
                case PROGRAM_STATE_PLACING_BODY:
                    bedit_process_placement_input(&program, &gui, &sim);
                    break;
                case PROGRAM_STATE_TEXT_INPUT:
                    // empty, because input processing will occur after render
                    break;
            }

            if (program.state != PROGRAM_STATE_MAIN_MENU)
                rndr_full_render(&program, &sim, &screen, &layerStatic, &gui);

            // Speed & FPS regulator
            if (sim.pausedByUser || !sim.fullSpeed)
                econio_sleep(program.sleepTime);

            // For scanf inputs
            if (program.state == PROGRAM_STATE_TEXT_INPUT) {
                switch (program.textInputDest) {
                    case TEXT_INPUT_BODY_EDITOR:
                        program.error = bedit_process_text_input(&program, &gui, &sim);
                        break;
                    case TEXT_INPUT_IMPORT:
                    case TEXT_INPUT_EXPORT:
                        program.error = fs_saving_process_text_input(&gui, &program, &sim, &screen, &layerStatic.layerInstances);
                        break;
                }
            }
        }

        // Handle error
        if (program.error != SUCCESS) {
            err_render(program.error, &screen, &layerStatic.layerInstances);
            rndr_refresh_screen(&program, &sim, &screen, &layerStatic);

            err_await_confirmation();

            if (program.error == ERR_MEMORY)
                program.exiting = true;
            else
                program.error = SUCCESS;
        }
    }


    exit_program(&layerStatic, sim, &screen);

    return 0;
}


static void init_modules(Program *program, LayerStatic *ls, Simulation *sim, Screen *screen) {
    if (lyr_init(&ls->layerInstances, ls->layers, screen) != SUCCESS ||
        rndr_init(screen) != SUCCESS ||
        bdy_init(sim) != SUCCESS) {
        program->error = ERR_MEMORY;
    }
}


static void exit_program(LayerStatic *ls, Simulation sim, Screen *screen) {
    econio_clrscr();
    econio_gotoxy(0, 0);
    printf("Exiting...\n\n");

    lyr_dispose(ls->layers);
    rndr_dispose(screen);
    barr_dispose(&sim.bodyArray);
}