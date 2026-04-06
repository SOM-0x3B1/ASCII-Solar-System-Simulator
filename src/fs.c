#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "sim/body.h"
#include "graphics/drawing.h"

#include "gui/edit_menu.h"


#define MAX_FILENAME_LENGTH 250

/** Copies the string name of the parameter into the destination from the source. */
static void get_param(char *dest, const char *src) {
    int i = 0, j = 0;
    for (; src[i] != '\0' && src[i] != '='; ++i)
        dest[j++] = src[i];
    dest[j] = '\0';
}

/** Copies the value string into the destination from the source. */
static void copy_string_value(char *dest, const char *src) {
    int i = 0, j = 0;
    while (src[i] != '\0' && src[i] != '=')
        i++;
    i++;
    for (; src[i] != '\0'; ++i)
        dest[j++] = src[i];
    dest[j] = '\0';
}

/** Scans the input file name for illegal characters. */
static int check_file_name(const char *fn) {
    int i = 0;
    for (; fn[i] != '\0' && i < MAX_FILENAME_LENGTH; ++i) {
        char c = fn[i];
        if (c != '-' && c != '_' && c != '.' && (c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a') || (c > 'z')))
            return 1; // invalid character
    }
    if (fn[i] != '\0')
        return 2; // invalid length

    return 0;
}

/**
 * Creates a new file, and writes the content of the body array into it.
 * @return Success / error
 */
static Error export(char *filename, Simulation *sim) {
    FILE *f;
    f = fopen(strcat(filename, ".tsv"), "w");
    if (f != NULL) {
        fprintf(f, "solar-mass\n%lf\n\n", sim->solarMass);

        fprintf(f, "name\tmass\tpos-x\tpos-y\tradius\tvel-x\tvel-y\tcolor");
        for (int i = 0; i < sim->bodyArray.length; ++i) {
            Body *b = &sim->bodyArray.data[i];
            fprintf(f, "\n%s\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%c",
                    b->name, b->mass, b->position.x, b->position.y,
                    b->r, b->velocity.x, b->velocity.y, b->color);
        }
        fclose(f);
    } else
        return ERR_EXPORT_WRITE; // unable to create file

    return SUCCESS;
}

/**
 * Creates a new file, and writes the content of the body array into it.
 * @return Success / error
 */
static Error import(char *filename, Simulation *sim, Program *program, Gui *gui, Screen *screen, LayerInstances *li) {
    FILE *f;
    f = fopen(strcat(filename, ".tsv"), "r");
    if (f != NULL) {
        char line[120];
        for (int i = 0; i < 2; ++i) {
            if(fgets(line, 120, f) == NULL) {
                fclose(f);
                return ERR_IMPORT_VALUE;
            }
        }

        barr_dispose(&sim->bodyArray);
        barr_init(&sim->bodyArray);

        double value;
        if(sscanf(line, "%lf", &value) != 1) {
            fclose(f);
            return ERR_IMPORT_VALUE;
        }
        if(value <= 0) {
            fclose(f);
            return ERR_IMPORT_VALUE;
        }
        sim->solarMass = value;

        for (int i = 0; i < 2; ++i) {
            if(fgets(line, 120, f) == NULL) {
                fclose(f);
                return ERR_IMPORT_VALUE;
            }
        }

        Body b;
        int res = 8;
        while (res == 8){
            res = fscanf(f, " %12[^\t]\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%c ",
                   b.name, &b.mass, &b.position.x, &b.position.y,
                   &b.r, &b.velocity.x, &b.velocity.y, &b.color);
            if (res == 8) {
                Body *nb = bdy_new(b.name, b.position, b.velocity, b.r, b.mass, b.color, sim);
                nb->name[12] = '\0';
                if(sim->bodyArray.length == 1) {
                    sim->sun = nb;
                    sim->following = nb;
                }
            } else if(res == EOF)
                break;
            else {
                fclose(f);
                return ERR_IMPORT_VALUE;
            }
        }
        fclose(f);
    } else
        return ERR_IMPORT_OPEN_FILE; // unable to create file

    editm_close(program, gui, li, screen, sim);

    return SUCCESS;
}


Error fs_load_settings(Simulation *sim, Screen *screen) {
    FILE *f;
    f = fopen("settings.ini", "r");

    if (f != NULL) {
        char line[32];
        while (fscanf(f, "%31s", line) != EOF) {
            char param[32];
            char sValue[32];

            get_param(param, line);
            copy_string_value(sValue, line);

            double value = 0;
            if (sscanf(sValue, "%lf", &value) != 1) {
                fclose(f);
                return ERR_SETTIGNS_VALUE; // invalid value
            }

            if (strcmp(param, "screen_width") == 0) {
                if (value > 0) screen->width = (int) value;
                else {
                    fclose(f);
                    return ERR_SETTIGNS_VALUE;
                }
            } else if (strcmp(param, "screen_height") == 0) {
                if (value > 0) screen->height = (int) value;
                else {
                    fclose(f);
                    return ERR_SETTIGNS_VALUE;
                }
            } else if (strcmp(param, "targetFPS") == 0) {
                if (value > 0) screen->targetFPS = (int) value;
                else {
                    fclose(f);
                    return ERR_SETTIGNS_VALUE;
                }
            } else if (strcmp(param, "solarMass") == 0) {
                if (value > 0) sim->solarMass = value;
                else {
                    fclose(f);
                    return ERR_SETTIGNS_VALUE;
                }
            } else if (strcmp(param, "detectCollisionPercentage") == 0) {
                if (value > 0) sim->detectCollisionPercentage = (double) value / 100;
                else {
                    fclose(f);
                    return ERR_SETTIGNS_VALUE;
                }
            } else {
                fclose(f);
                return ERR_SETTINGS_PARAMETER;
            }
        }
        fclose(f);
    } else
        return ERR_SETTINGS_OPEN_FILE;

    return SUCCESS;
}

Error fs_load_main_menu(Gui *gui) {
    FILE *f;
    f = fopen("earth-animation.txt", "r");

    if (f == NULL)
        return ERR_MAIN_MENU_ART_MISSING;

    for (int i = 0; i < EARTH_ANIMATION_FRAMES; ++i) {
        for (int y = 0; y < EARTH_ANIMATION_HEIGHT; ++y) {
            fgets(gui->mainMenu_animation[i][y], EARTH_ANIMATION_WIDTH + 1, f);
            gui->mainMenu_animation[i][y][60] = '\0';
        }
    }
    fclose(f);
    f = NULL;

    f = fopen("title.txt", "r");
    if (f == NULL)
        return ERR_MAIN_MENU_ART_MISSING;

    for (int y = 0; y < 13; ++y) {
        fgets(gui->mainMenu_title[y], 60, f);
        gui->mainMenu_title[y][51] = '\0';
    }
    fclose(f);

    return SUCCESS;
}

void fs_export_switch(Program *program) {
    program->state = PROGRAM_STATE_TEXT_INPUT;
    program->textInputDest = TEXT_INPUT_EXPORT;
}

void fs_export_render(Gui *gui, LayerInstances *li, Screen *screen) {
    gui->textPos = drw_draw_input_prompt(&li->menuLayer, screen->height / 2 - 2, "Export system", "Name:", screen);
}

void fs_import_switch(Program *program) {
    program->state = PROGRAM_STATE_TEXT_INPUT;
    program->textInputDest = TEXT_INPUT_IMPORT;
}

void fs_import_render(Gui *gui, LayerInstances *li, Screen *screen) {
    gui->textPos = drw_draw_input_prompt(&li->menuLayer, screen->height / 2 - 2, "Import system", "Name:", screen);
}

Error fs_saving_process_text_input(Gui *gui, Program *program, Simulation *sim, Screen *screen, LayerInstances *li) {
    econio_gotoxy((int) gui->textPos.x, (int) gui->textPos.y);
    econio_normalmode();

    char filename[MAX_FILENAME_LENGTH + 5];
    char tempFormat[6];
    sprintf(tempFormat, "%%%ds", MAX_FILENAME_LENGTH);
    scanf(tempFormat, filename);
    while (getchar() != '\n');

    econio_rawmode();
    econio_gotoxy(0, 0);

    program->state = PROGRAM_STATE_EDIT_MENU;

    if (check_file_name(filename) == 0) {
        if(program->textInputDest == TEXT_INPUT_EXPORT)
            return export(filename, sim);
        else
            return import(filename, sim, program, gui, screen, li);
    }
    else
        return ERR_FS_FILENAME;
}
