#include "drawing.h"

#include <string.h>

#include "../gui/edit_menu.h"


static inline void swap_ints(int *a, int *b) {
    const int temp = *a;
    *a = *b;
    *b = temp;
}

/** Swaps two points so that the second one becomes the greater one. */
static inline void sort_points(int x1, int y1, int x2, int y2) {
    if (x1 > x2)
        swap_ints(&x1, &x2);
    if (y1 > y2)
        swap_ints(&y1, &y2);
}


void drw_draw_text(Layer *l, int x, int y, const char *s, Screen *screen) {
    for (int i = 0; s[i] != '\0' && x + i < screen->width; ++i)
        lyr_write_at_xy(l, x + i, y, s[i], screen);
}


void drw_draw_line(Layer *l, int x, int y, int length, bool vertical, char c, Screen *screen) {
    if (vertical) {
        for (int i = y; i <= y + length && i < screen->height; ++i)
            lyr_write_at_xy(l, x, i, c, screen);
    } else {
        for (int i = x; i <= x + length && i < screen->width; ++i)
            lyr_write_at_xy(l, i, y, c, screen);
    }
}


void drw_draw_rectangle_fill(Layer *l, int x1, int y1, int x2, int y2, char c, Screen *screen) {
    sort_points(x1, y1, x2, y2);
    for (int y = y1; y <= y2 && y < screen->height; ++y) {
        for (int x = x1; x <= x2 && x < screen->width; ++x)
            lyr_write_at_xy(l, x, y, c, screen);
    }
}

void drw_draw_rectangle_outline(Layer *l, int x1, int y1, int x2, int y2, Screen *screen) {
    drw_draw_line(l, x1 + 1, y1, x2 - x1 - 2, false, '_', screen);
    drw_draw_line(l, x1, y2, x2 - x1, false, '_', screen);
    drw_draw_line(l, x1, y1 + 1, y2 - y1 - 1, true, '|', screen);
    drw_draw_line(l, x2, y1 + 1, y2 - y1 - 1, true, '|', screen);
}


int drw_draw_box(Layer *l, int x1, int y1, int x2, int y2, const char *title, Screen *screen) {
    sort_points(x1, y1, x2, y2);

    int xCenter = x1 + (x2 - x1) / 2;

    //drawing_drawRectangleOutline(l, x1 - 1, y1 + 1, x2 + 1, y2+1, screen);
    drw_draw_rectangle_fill(l, x1 + 1, y1 + 1, x2 - 1, y2 - 1, ' ', screen);
    drw_draw_rectangle_outline(l, x1, y1, x2, y2, screen);

    drw_draw_line(l, x1 + 1, y1 + 2, x2 - x1 - 2, false, '_', screen);

    drw_draw_text(l, xCenter - (int) strlen(title) / 2, y1 + 1, title, screen);

    return xCenter;
}


Point drw_draw_input_prompt(Layer *l, int yCenter, const char *title, const char *text, Screen *screen) {
    Point textPos;
    int margin = screen->width / 4;
    int xCentrer = drw_draw_box(l, margin - EDIT_MENU_WIDTH / 2, yCenter - 4,
                                   screen->width - margin - EDIT_MENU_WIDTH / 2, yCenter + 4,
                                   title, screen);

    drw_draw_text(l, margin - EDIT_MENU_WIDTH / 2 + 3, yCenter + 1, text, screen);
    textPos = (Point) {margin + 3 - EDIT_MENU_WIDTH / 2 + (int) strlen(text) + 2, yCenter + 1};

    drw_draw_text(l, xCentrer - 12, yCenter + 3, "Press 'ENTER' to accept", screen);

    return textPos;
}