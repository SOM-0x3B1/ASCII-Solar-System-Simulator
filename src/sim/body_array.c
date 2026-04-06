#include "body_array.h"


/**
 * If the body array shifts or gets relocated, the global body variables must be updated to their new pointers.
 * This function checks and updates a single body pointer.
 * @param array The new/shifted body array
 * @param origin Original index of the body.
 * @param newIndex The new index of the body.
 */
static void update_pointers(Body *array, int origin, int newIndex, Simulation *sim) {
    if (&sim->bodyArray.data[origin] == sim->following)
        sim->following = &array[newIndex];
    if (&sim->bodyArray.data[origin] == sim->sun)
        sim->sun = &array[newIndex];
    if (&sim->bodyArray.data[origin] == sim->editedBody)
        sim->editedBody = &array[newIndex];
}


Error barr_init(BodyArray *ba) {
    ba->length = 0;
    ba->capacity = 1;
    ba->data = (Body *) malloc(sizeof(Body));
    if (ba->data == NULL)
        return ERR_MEMORY;
    return SUCCESS;
}

void barr_dispose(BodyArray *ba) {
    for (int i = 0; i < ba->length; ++i)
        bdy_trail_clear(&ba->data[i].trail);
    free(ba->data);
}

Body *barr_add(BodyArray *ba, Body *b, Simulation *sim) {
    if (sim->bodyArray.length + 1 > sim->bodyArray.capacity) {
        //Body *newArray = realloc(bodyArray.data, bodyArray.capacity * 2 * sizeof(Body));
        Body *newArray = (Body *) malloc(sim->bodyArray.capacity * 2 * sizeof(Body));
        if (newArray == NULL)
            return NULL;  // failed to allocate memory for body
        else {
            for (int i = 0; i < sim->bodyArray.length; ++i) {
                newArray[i] = sim->bodyArray.data[i];
                update_pointers(newArray, i, i, sim);
            }
            free(sim->bodyArray.data);
            sim->bodyArray.data = newArray;
            sim->bodyArray.capacity *= 2;
        }
    }

    ba->data[ba->length] = *b;
    ba->length++;

    return &ba->data[ba->length - 1];
}


void barr_remove_at(BodyArray *ba, int i, Simulation *sim) {
    if (sim->following == &ba->data[i])
        sim->following = NULL;

    bdy_trail_clear(&ba->data[i].trail);

    for (int j = i; j < ba->length - 1; ++j) {
        ba->data[j] = ba->data[j + 1];
        update_pointers(ba->data, j + 1, j, sim);
    }
    ba->length--;
}


void barr_remove(BodyArray *ba, Body *b, Simulation *sim) {
    int i = 0;
    while (&ba->data[i] != b && i < ba->length)
        i++;
    if (i < ba->length)
        barr_remove_at(ba, i, sim);
}