#include "simulator.h"

#include <stdlib.h>

#include "body.h"
#include "../gui/edit_menu.h"


/** Processes the gravitatinal interactions of the bodies, and updates their velocity. */
static void do_gravity_calculations(BodyArray *ba, Simulation *sim) {
    for (int i = 0; i < ba->length; ++i) {
        for (int j = 0; j < ba->length; ++j) {
            if (i != j)
                bdy_add_g_effect(&ba->data[i], &ba->data[j], sim);
        }
    }
}

/** Moves the bodies. */
static void do_movements(Simulation *sim) {
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        bdy_move(&sim->bodyArray.data[i]);
        if (sim->trailSpacingCounter > 15)
            bdy_trail_enqueue(&sim->bodyArray.data[i].trail, sim->bodyArray.data[i].position);
    }
    if (sim->trailSpacingCounter > 15)
        sim->trailSpacingCounter = 0;
    sim->trailSpacingCounter++;
}

/** Checks if any of the bodies is colliding with another body.  */
static void detect_collisions(BodyArray *ba, Simulation *sim) {
    for (int i = 0; i < ba->length - 1; ++i) {
        for (int j = i + 1; j < ba->length; ++j) {
            Body *a = &ba->data[i];
            Body *b = &ba->data[j];
            bdy_detectCollision(a, b, sim);
        }
    }
}

/**
 * Moves the camera in accordance to the input key.
 * @return Has the camare been moved
 */
static bool simulation_move_camera(EconioKey key, Screen *screen) {
    if (key == 's' || key == KEY_DOWN) {
        screen->offset.y++;
        return true;
    } else if (key == 'w' || key == KEY_UP) {
        screen->offset.y--;
        return true;
    } else if (key == 'a' || key == KEY_LEFT) {
        screen->offset.x -= 2;
        return true;
    } else if (key == 'd' || key == KEY_RIGHT) {
        screen->offset.x += 2;
        return true;
    }
    return false;
}


void sim_tick(Simulation *sim) {
    if (!sim->pausedByUser) {
        do_gravity_calculations(&sim->bodyArray, sim);
        do_movements(sim);
        detect_collisions(&sim->bodyArray, sim);
    }
}

void sim_process_input(Simulation *sim, Screen *screen, Program *program, Gui *gui, LayerInstances *li) {
    if (econio_kbhit()) {
        int key = 0;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ESCAPE || key == 'e')
            editm_switch(key, program, gui, screen, li);
        else if (key == ' ') {
            sim->pausedByUser = !sim->pausedByUser;
        } else if (key == 'q') {
            sim->fullSpeed = !sim->fullSpeed;
            if (!sim->fullSpeed)
                rndr_reset_fps_measurement(screen);
        } else if (simulation_move_camera(key, screen))
            sim->following = NULL;
    }
}