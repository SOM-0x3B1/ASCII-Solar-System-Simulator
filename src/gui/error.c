#include "error.h"

#include <string.h>

#include "../graphics/drawing.h"
#include "../lib/econio.h"


/** Converts an error code into an informative string message. */
static const char* err_to_string(Error error) {
    switch (error) {
        case ERR_MEMORY:                return "CRITICAL ERROR: Unable to allccate memory.";
        case ERR_SETTINGS_OPEN_FILE:    return "Unable to open settings.ini.";
        case ERR_SETTINGS_PARAMETER:    return "Invalid parameter name in settings.";
        case ERR_SETTIGNS_VALUE:        return "Invalid value in settings.";
        case ERR_FS_FILENAME:           return "Invalid file name.";
        case ERR_ADDBODY_VALUE:         return "Invalid body parameter.";
        case ERR_EXPORT_WRITE:          return "Unable to create export file.";
        case ERR_IMPORT_OPEN_FILE:      return "Unable to open import file.";
        case ERR_IMPORT_VALUE:          return "Invalid body parameter in import file.";
        case ERR_MAIN_MENU_ART_MISSING: return "Missing main menu art files.";
        default:                        return "Unkown error.";
    }
}

void err_render(Error error, Screen *screen, LayerInstances *li) {
    const char* errString = err_to_string(error);
    int msgLength = (int) strlen(errString);

    if (msgLength < 26)
        msgLength = 26;

    int x1 = screen->width / 2 - msgLength / 2 - 3;
    int yCenter = screen->height / 2 + 1;

    drw_draw_rectangle_outline(&li->overlayLayer, x1 - 1, yCenter - 5,
                                 x1 + msgLength + 5, yCenter + 2, screen);
    int xCentrer = drw_draw_box(&li->overlayLayer, x1, yCenter - 6,
                                   x1 + msgLength + 4, yCenter + 1, "ERROR", screen);
    drw_draw_text(&li->overlayLayer, x1 + 2, yCenter - 2, errString, screen);
    drw_draw_text(&li->overlayLayer,
                     xCentrer - (error != ERR_MEMORY ? 13 : 10), yCenter,
                     error != ERR_MEMORY ? "Press 'ENTER' to continue" : "Press 'ENTER' to abort", screen);
}


void err_await_confirmation() {
    int key = 0;
    econio_rawmode();
    while (key != KEY_ENTER) {
        if (econio_kbhit()) {
            while (econio_kbhit())
                key = econio_getch();
            econio_sleep(0.02);
        }
    }
}