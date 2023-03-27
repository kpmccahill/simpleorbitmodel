#ifndef BODY_H
#define BODY_H

#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define GRAVITY_CONST 0.000000000066743 // gravitational constatnt :) real
// #define GRAVITY_CONST .0006674 // gravitational constatnt :) scaled?

struct Body 
{
    Vector2 position;
    Vector2 velocity;
    double mass;
    double radius;
};

extern double distance_to_body(Vector2 reference, Vector2 target);

extern double caluclate_attractive_force(struct Body reference, struct Body target);

extern double calculate_force_acceleration(double attractive_force, struct Body body);

Vector2 caluclate_orbit_vector(struct Body reference, struct Body target);

#endif