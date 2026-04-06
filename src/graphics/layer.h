#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H

#include <stdbool.h>
#include "render.h"
#include "../structs.h"
#include "../lib/econio.h"


/** Initializes the layer arrays.
 *  @return Successful / error
 */
Error lyr_init(LayerInstances *li, Layer **layers, Screen *screen); //

/** Frees the layer arrays. */
void lyr_dispose(Layer **layers);

/**
 * Writes a single character into a layer.
 * @param l Layer
 * @param c character
 */
void lyr_write_at_xy(Layer *l, int x, int y, char c, Screen *screen);


/**
 * Replaces every character in all of the layers with \0.
 * @param l Layer
 */
void lyr_clear_all(Layer **l, Screen *screen);


#endif //ASCII_SSS_LAYER_H