#ifndef ASCII_SSS_DRAWING_H
#define ASCII_SSS_DRAWING_H

#include "layer.h"
#include "render.h"
#include "../structs.h"
#include "../vector.h"
#include "../lib/econio.h"


/**
 * Writes a string onto a layer.
 * @param l Layer
 * @param s string
 */
void drw_draw_text(Layer *l, int x, int y, const char *s, Screen *screen);

/**
 * Draws a line.
 * @param l Layer
 * @param length The length of the line
 * @param vertical Is the line vertical
 * @param c The characters of which the line should be built of
 */
void drw_draw_line(Layer *l, int x, int y, int length, bool vertical, char c, Screen *screen);


/**
 * Draws a filled rectangle without outline.
 * @param l Layer
 * @param x1 Top left (x)
 * @param y1 Top left (y)
 * @param x2 Bottom right (x)
 * @param y2 Bottom right (y)
 * @param c Fill character
 */
void drw_draw_rectangle_fill(Layer *l, int x1, int y1, int x2, int y2, char c, Screen *screen);

/**
 * Draws an unfilled rectangle (outline only).
 * @param l Layer
 * @param x1 Top left (x)
 * @param y1 Top left (y)
 * @param x2 Bottom right (x)
 * @param y2 Bottom right (y)
 */
void drw_draw_rectangle_outline(Layer *l, int x1, int y1, int x2, int y2, Screen *screen);

/**
 * Draws a window (box) with a title.
 * @param l Layer
 * @param x1 Top left (x)
 * @param y1 Top left (y)
 * @param x2 Bottom right (x)
 * @param y2 Bottom right (y)
 * @return The horizontal (x) position of the center of the box
 */
int drw_draw_box(Layer *l, int x1, int y1, int x2, int y2, const char *title, Screen *screen);

/**
 * Draws a text input box.
 * @param l Layer
 * @param yCenter The vertical center of the box
 * @param title The title of the box
 * @param text The prompt text
 * @return The coords of the cursor (next to the prompt text)
 */
Point drw_draw_input_prompt(Layer *l, int yCenter, const char *title, const char *text, Screen *screen);


#endif //ASCII_SSS_DRAWING_H
