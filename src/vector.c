#include "vector.h"
#include "math.h"

/**
 * Raises a number to the power of 2.
 * @param x base
 * @return base^2
 */
static double pow2(double x) {
    return x * x;
}


Vector vec_create(double length, double angle) {
    Vector v;
    double rad = angle * (3.14159265359 / 180.0);
    v.x = length * cos(rad);
    v.y = length * sin(rad);
    return v;
}

Vector vec_add(Vector u, Vector v) {
    Vector res = {u.x + v.x, u.y + v.y};
    return res;
}

Vector vec_subtract(Vector u, Vector v) {
    Vector res = {u.x - v.x, u.y - v.y};
    return res;
}

Vector vec_scalar_multiply(Vector u, double m) {
    Vector res = {u.x * m, u.y * m};
    return res;
}

Vector vec_scalar_divide(Vector u, double d) {
    Vector res = {u.x / d, u.y / d};
    return res;
}

Vector vec_invert(Vector a) {
    Vector res = {-(a.x), -(a.y)};
    return res;
}

double vec_distance(Vector u, Vector v) {
    return sqrt(pow2(u.x - v.x) + pow2(u.y - v.y));
}

/*double vector_angle(Vector u, Vector v){
    double m = vector_multiply(u, v);
    double dU = vector_length(u);
    double dV = vector_length(v);

    return acos(m / (dU * dV));
}*/

Vector vec_unit_vector(Vector u, Vector v) {
    Vector w = vec_subtract(u, v);
    double d = vec_distance(u, v);
    return vec_scalar_divide(w, d);
}

Point vec_to_point(Vector a) {
    Point p = {(long long int) round(a.x), (long long int) round(a.y)};
    return p;
}


Point pnt_subtract(Point p, Point q) {
    Point res = {p.x - q.x, p.y - q.y};
    return res;
}

Point pnt_scalar_multiply(Point p, int m) {
    Point res = {p.x * m, p.y * m};
    return res;
}

Point pnt_scalar_divide(Point p, int d){
    Point res = {p.x / d, p.y / d};
    return res;
}

Vector pnt_to_vector(Point p){
    Vector v = (Vector){(double)p.x, (double)p.y};
    return v;
}