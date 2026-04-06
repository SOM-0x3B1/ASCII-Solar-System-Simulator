#include "edit_menu.h"

#include <string.h>

#include "body_editor.h"
#include "../fs.h"
#include "../graphics/drawing.h"


static inline void main_option_to_string(char *res, EditMenuMainOption o) {
    switch (o) {
        case OPTION_ADD_BODY:
            strcpy(res, "Add body");
            break;
        case OPTION_EDIT_BODY:
            strcpy(res, "Edit body");
            break;
        case OPTION_DELETE_BODY:
            strcpy(res, "Delete body");
            break;
        case OPTION_FOLLOW_BODY:
            strcpy(res, "Follow body");
            break;
        case OPTION_TOGGLE_DETAILS:
            strcpy(res, "Toggle details");
            break;
        case OPTION_TOGGLE_G_RANGE:
            strcpy(res, "Toggle G range");
            break;
        case OPTION_TOGGLE_TRAILS:
            strcpy(res, "Toggle trails");
            break;
        case OPTION_IMPORT_SYSTEM:
            strcpy(res, "Import system");
            break;
        case OPTION_EXPORT_SYSTEM:
            strcpy(res, "Export system");
            break;
        default:
            strcpy(res, "Exit");
            break;
    }
}

static inline void body_option_to_string(char *res, BodyEditableProperty p) {
    switch (p) {
        case BODY_PROPERTY_NAME:
            strcpy(res, "Set name");
            break;
        case BODY_PROPERTY_MASS:
            strcpy(res, "Set mass");
            break;
        case BODY_PROPERTY_R:
            strcpy(res, "Set radius");
            break;
        case BODY_PROPERTY_POS:
            strcpy(res, "Set position");
            break;
        case BODY_PROPERTY_VEL:
            strcpy(res, "Set velocity");
            break;
        default:
            strcpy(res, "");
            break;
    }
}


/** Renders the "[ON]"/"[OFF]" texts next to toggle buttons. */
static inline void render_toggle_option_state(int y, bool stat, LayerInstances *li, Screen *screen) {
    drw_draw_text(&li->menuLayer, screen->width - 13, y, stat ? "[ON] " : "[OFF]", screen);
}

/** Renders the '>' sign next to the selected option. */
static inline void render_selection(int i, int y, Gui *gui, LayerInstances *li, Screen *screen) {
    if (i == gui->cursorPos)
        drw_draw_text(&li->menuLayer, screen->width - 30, y, ">", screen);
    else
        drw_draw_text(&li->menuLayer, screen->width - 30, y, " ", screen);
}

/** Renders the list of main options. */
static void render_main(LayerInstances *li, Gui *gui, Screen *screen) {
    int yOffset = 0;
    for (int i = 0; i < EditMenuMainOption_MAX; ++i) {
        if (i > OPTION_EXPORT_SYSTEM)
            yOffset = 3;
        else if (i > OPTION_TOGGLE_TRAILS)
            yOffset = 2;
        else if (i > OPTION_DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        char sMainOption[20];
        main_option_to_string(sMainOption, i);
        drw_draw_text(&li->menuLayer, screen->width - 28, y, sMainOption, screen);
        render_selection(i, y, gui, li, screen);

        if (i == OPTION_TOGGLE_DETAILS)
            render_toggle_option_state(y, li->infoLayer.enabled, li, screen);
        else if (i == OPTION_TOGGLE_G_RANGE)
            render_toggle_option_state(y, li->rangeLayer.enabled, li, screen);
        else if (i == OPTION_TOGGLE_TRAILS)
            render_toggle_option_state(y, li->trailLayer.enabled, li, screen);
    }
}

/** Renders the list of bodies. */
static void render_body_list(Simulation *sim, LayerInstances *li, Screen *screen, Gui *gui) {
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        int y = 5 + i;

        drw_draw_text(&li->menuLayer, screen->width - 28, y, sim->bodyArray.data[i].name, screen);
        render_selection(i, y, gui, li, screen);
    }

    drw_draw_text(&li->menuLayer, screen->width - 28, 5 + sim->bodyArray.length + 1, "Back", screen);
    render_selection(sim->bodyArray.length, 5 + sim->bodyArray.length + 1, gui, li, screen);
}

/** Renders the list of edit body properties. */
static void render_edit_properties(LayerInstances *li, Screen *screen, Simulation *sim, Gui *gui) {
    drw_draw_text(&li->menuLayer, screen->width - 30, 5, sim->editedBody->name, screen);

    for (int i = 0; i < bodyEditableProperty_MAX; ++i) {
        int y = 6 + i;

        char sBodyOption[15];
        body_option_to_string(sBodyOption, i);
        drw_draw_text(&li->menuLayer, screen->width - 28, y, sBodyOption, screen);
        render_selection(i, y, gui, li, screen);
    }

    drw_draw_text(&li->menuLayer, screen->width - 28, 6 + bodyEditableProperty_MAX + 1, "Accept", screen);
    render_selection(bodyEditableProperty_MAX, 6 + bodyEditableProperty_MAX + 1, gui, li, screen);
}


/** Enter on main menu option. */
static Error select_main_option(Gui *gui, Simulation *sim, Screen *screen, LayerInstances *li, Program *program) {
    EditMenuSTATE lastState = gui->editMenu_state;

    switch (gui->cursorPos) {
        case OPTION_ADD_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_ADD_BODY;
            gui->bodyEditor_state = BODY_SET_NAME;
            bedit_switch(program);
            sim->editedBody =
                    bdy_new("",
                             (Vector) {(double) screen->offset.x + (double) screen->width / 2 -
                                       (double) EDIT_MENU_WIDTH / 2,
                                       ((double) screen->offset.y + (double) screen->height / 2) * 2},
                             (Vector) {0, 0}, 0.0, 0.0, '#', sim);
            if (sim->editedBody == NULL) {
                program->exiting = true;
                return ERR_MEMORY;
            }
            sim->following = sim->editedBody;
            break;
        case OPTION_DELETE_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_DELETE_BODY;
            break;
        case OPTION_EDIT_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY;
            break;
        case OPTION_FOLLOW_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_FOLLOW_BODY;
            break;
        case OPTION_TOGGLE_DETAILS:
            li->infoLayer.enabled = !li->infoLayer.enabled;
            break;
        case OPTION_TOGGLE_G_RANGE:
            li->rangeLayer.enabled = !li->rangeLayer.enabled;
            break;
        case OPTION_TOGGLE_TRAILS:
            li->trailLayer.enabled = !li->trailLayer.enabled;
            break;
        case OPTION_EXPORT_SYSTEM:
            fs_export_switch(program);
            break;
        case OPTION_IMPORT_SYSTEM:
            fs_import_switch(program);
            break;
        case OPTION_EXIT:
            program->exiting = true;
            break;
        default:
            break;
    }

    if (lastState != gui->editMenu_state)
        gui->cursorPos = 0;

    return SUCCESS;
}

/** Enter on "Edit body"/[body] */
static void select_edit_option(Program *p, Gui *gui, Simulation *sim) {
    if (gui->cursorPos == sim->bodyArray.length) {
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else {
        sim->editedBody = &sim->bodyArray.data[gui->cursorPos];
        sim->following = sim->editedBody;
        gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY_SET;
        gui->cursorPos = 0;
    }
}

/** Enter on "Edit body"/[body]/[property] */
static void select_edit_property_option(Gui *gui, Program *program) {
    if (gui->cursorPos == bodyEditableProperty_MAX) {
        gui->cursorPos = 0;
        gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY;
    } else {
        gui->bodyEditor_state = gui->cursorPos;
        if (gui->cursorPos != BODY_SET_POS)
            bedit_switch(program);
        else {
            program->textInputDest = TEXT_INPUT_BODY_EDITOR;
            program->state = PROGRAM_STATE_PLACING_BODY;
        }
    }
}

/** Enter on "Delete body"/[body] */
static void select_delete_option(Gui *gui, Simulation *sim) {
    if (gui->cursorPos == sim->bodyArray.length) {
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else {
        if (sim->following == &sim->bodyArray.data[gui->cursorPos])
            sim->following = NULL;
        barr_remove_at(&sim->bodyArray, gui->cursorPos, sim);
        if (gui->cursorPos == sim->bodyArray.length && gui->cursorPos > 0)
            gui->cursorPos--;
    }
}

/** Enter on "Follow body"/[body] */
static void select_follow_option(Gui *gui, Simulation *sim) {
    if (gui->cursorPos == sim->bodyArray.length) {
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else
        sim->following = &sim->bodyArray.data[gui->cursorPos];
}


static inline int get_option_count(EditMenuSTATE s) {
    switch (s) {
        case EDIT_MENU_STATE_MAIN:
            return EditMenuMainOption_MAX;
        case EDIT_MENU_STATE_ADD_BODY:
        case EDIT_MENU_STATE_EDIT_BODY_SET:
            return bodyEditableProperty_MAX + 1;
        default:
            return -1;
    }
}


void editm_switch(EconioKey key, Program *program, Gui *gui, Screen *screen, LayerInstances *li) {
    program->state = PROGRAM_STATE_EDIT_MENU;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;

    li->menuLayer.enabled = true;
    screen->offset.x += EDIT_MENU_WIDTH / 2;

    if (key == KEY_ESCAPE)
        gui->cursorPos = OPTION_EXIT;
}

void editm_render(LayerInstances *li, Screen *screen, Gui *gui, Simulation *sim) {
    drw_draw_line(&li->menuLayer, screen->width - 32, 2,
                     screen->height - 4, true, '|', screen);
    drw_draw_rectangle_fill(&li->menuLayer, screen->width - 31, 2,
                              screen->width - 1, screen->height - 2, ' ', screen);
    drw_draw_text(&li->menuLayer, screen->width - 30, 3, "[EDIT MENU]", screen);

    switch (gui->editMenu_state) {
        case EDIT_MENU_STATE_MAIN:
            render_main(li, gui, screen);
            break;
        case EDIT_MENU_STATE_ADD_BODY:
        case EDIT_MENU_STATE_EDIT_BODY:
        case EDIT_MENU_STATE_DELETE_BODY:
        case EDIT_MENU_STATE_FOLLOW_BODY:
            render_body_list(sim, li, screen, gui);
            break;
        case EDIT_MENU_STATE_EDIT_BODY_SET:
            render_edit_properties(li, screen, sim, gui);
            break;
        default:
            break;
    }
}

/** Closes the edit menu and returns to simulation mode. */
void editm_close(Program *program, Gui *gui, LayerInstances *li, Screen *screen, Simulation *sim) {
    program->state = PROGRAM_STATE_SIMULATION;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;
    li->menuLayer.enabled = false;
    gui->cursorPos = 0;
    editm_render(li, screen, gui, sim);

    screen->offset.x -= EDIT_MENU_WIDTH / 2;
}

Error editm_process_input(Program *program, Simulation *sim, Screen *screen, Gui *gui, LayerInstances *li) {
    int key = 0;
    if (econio_kbhit()) {
        while (econio_kbhit())
            key = econio_getch();

        int cursorMax = get_option_count(gui->editMenu_state);
        if (cursorMax < 0)
            cursorMax = sim->bodyArray.length + 1;

        if (key == KEY_ESCAPE || key == 'e')
            editm_close(program, gui, li, screen, sim);
        else if ((key == 's' || key == KEY_DOWN) && gui->cursorPos < cursorMax - 1)
            gui->cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && gui->cursorPos > 0)
            gui->cursorPos--;
        else if (key == KEY_ENTER || key == ' ') {
            switch (gui->editMenu_state) {
                case EDIT_MENU_STATE_MAIN:
                    return select_main_option(gui, sim, screen, li, program);
                case EDIT_MENU_STATE_EDIT_BODY:
                    select_edit_option(program, gui, sim);
                    break;
                case EDIT_MENU_STATE_EDIT_BODY_SET:
                    select_edit_property_option(gui, program);
                    break;
                case EDIT_MENU_STATE_DELETE_BODY:
                    select_delete_option(gui, sim);
                    break;
                case EDIT_MENU_STATE_FOLLOW_BODY:
                    select_follow_option(gui, sim);
                    break;
                default:
                    break;
            }
        }
    }
    return SUCCESS;
}