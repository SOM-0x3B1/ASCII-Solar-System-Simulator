#ifndef ASCII_SSS_VECTOR_H
#define ASCII_SSS_VECTOR_H

#include "structs.h"


/** Creates a vector from a length and an angle. */
Vector vec_create(double length, double angle);

/** Adds two vectors together. */
Vector vec_add(Vector u, Vector v);

/** Subracts two vectors from each other. */
Vector vec_subtract(Vector u, Vector v);

/** Multiplies a vector with a skalar. */
Vector vec_scalar_multiply(Vector u, double m);

/** Divides a vector with a skalar. */
Vector vec_scalar_divide(Vector u, double d);

/**
 * Inverts a vector.
 * @return (-a.x, -a.y)
 */
Vector vec_invert(Vector a);

/** Measures the distance between two coord. vectors. */
double vec_distance(Vector u, Vector v);

//double vector_angle(Vector u, Vector v);

/** Returns the unit vector of a vector between two coord. vectors. */
Vector vec_unit_vector(Vector u, Vector v);

/** Converts a Vector to a Point. */
Point vec_to_point(Vector a);


/** Subracts two vectors from each other. */
Point pnt_subtract(Point p, Point q);

/** Multiplies a vector with a skalar. */
Point pnt_scalar_multiply(Point p, int m);

Point pnt_scalarDivide(Point p, int d);

Vector pnt_ToVector(Point p);


#endif //ASCII_SSS_VECTOR_H