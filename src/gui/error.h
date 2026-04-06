#ifndef ASCII_SSS_ERROR_H
#define ASCII_SSS_ERROR_H

#include "../structs.h"


/** Renders an error message. */
void err_render(Error error, Screen *screen, LayerInstances *li);

/** Pauses the program until ENTER is pressed. */
void err_await_confirmation();


#endif //ASCII_SSS_ERROR_H
