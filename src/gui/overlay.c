#include "overlay.h"

#include <stdio.h>
#include <string.h>


/** Renders the FPS counter. */
static void render_fps(Screen *screen, LayerInstances *li) {
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", screen->fps, screen->targetFPS);
    drw_draw_text(&li->overlayLayer, 2, 0, sfps, screen);
}


/** Renders the title and the edit menu tips. */
static void render_header(Screen *screen, LayerInstances *li) {
    drw_draw_line(&li->overlayLayer, 0, 0, screen->width, false, ' ', screen);
    render_fps(screen, li);
    drw_draw_text(&li->overlayLayer, screen->width / 2 - 4, 0, "ASCII-SSS", screen);
    if (li->menuLayer.enabled)
        drw_draw_text(&li->overlayLayer, screen->width - 30, 0, "PRESS 'E' TO CLOSE EDIT MENU", screen);
    else
        drw_draw_text(&li->overlayLayer, screen->width - 29, 0, "PRESS 'E' TO OPEN EDIT MENU", screen);
    drw_draw_line(&li->overlayLayer, 0, 1, screen->width, false, '_', screen);
}


/** Returns a string describing the current state of the program. */
static void program_state_to_string(char *res, ProgramState s) {
    switch (s) {
        case PROGRAM_STATE_SIMULATION:
        case PROGRAM_STATE_EDIT_MENU:
            strcpy(res, "RUNNING");
            break;
        case PROGRAM_STATE_TEXT_INPUT:
            strcpy(res, "PAUSED (INPUT)");
            break;
        case PROGRAM_STATE_PLACING_BODY:
            strcpy(res, "PAUSED (PLACING)");
            break;
        case PROGRAM_STATE_MAIN_MENU:
            strcpy(res, "PAUSED (MAIN)");
            break;
    }
}


/** Renders the programstate, camera position and the body the camera is potentially following. */
static void render_footer(Program *program, Screen *screen, LayerInstances *li, Simulation *sim) {
    drw_draw_line(&li->overlayLayer, 0, screen->height - 3, screen->width, false, '_', screen);
    drw_draw_line(&li->overlayLayer, 0, screen->height - 2, screen->width, false, ' ', screen);
    drw_draw_text(&li->overlayLayer, 2, screen->height - 2, "Status:", screen);

    char sProgramState[30];
    program_state_to_string(sProgramState, program->state);

    if (!sim->pausedByUser ||
        (program->state != PROGRAM_STATE_SIMULATION && program->state != PROGRAM_STATE_EDIT_MENU)) {
        if (sim->fullSpeed && program->state == PROGRAM_STATE_SIMULATION)
            drw_draw_text(&li->overlayLayer, 10, screen->height - 2, "RUNNING (FULL SPEED)", screen);
        else
            drw_draw_text(&li->overlayLayer, 10, screen->height - 2, sProgramState, screen);
    } else
        drw_draw_text(&li->overlayLayer, 10, screen->height - 2, "PAUSED (BY USER)", screen);

    char *sCamPos[50];
    sprintf((char *) sCamPos, "Camera center: {%lld ; %lld}", screen->offset.x + screen->width / 2,
            -(screen->offset.y + screen->height / 2) * 2);
    drw_draw_text(&li->overlayLayer, screen->width / 2 - (int) strlen((char *) sCamPos) / 2, screen->height - 2,
                     (char *) sCamPos, screen);

    char *sFollowing[32];
    sprintf((char *) sFollowing, "Following: %s", (sim->following == NULL) ? "[FREE]" : sim->following->name);
    drw_draw_text(&li->overlayLayer, screen->width - (int) strlen((char *) sFollowing) - 2, screen->height - 2,
                     (char *) sFollowing, screen);

    drw_draw_line(&li->overlayLayer, 0, screen->height - 1, screen->width, false, ' ', screen);
}


void ovl_render(Program *program, Simulation *sim, Screen *screen, LayerInstances *li) {
    render_header(screen, li);
    render_footer(program, screen, li, sim);
}