#include "body.h"

extern double distance_to_body(Vector2 reference, Vector2 target)
{
    double distance;
    double dist_x = (double)pow((reference.x - target.x), 2);
    double dist_y = (double)pow((reference.y - target.y), 2);
    distance = sqrtf(dist_x + dist_y);

    return distance;
}

extern double caluclate_attractive_force(struct Body reference, struct Body target) {
    double distance = distance_to_body(reference.position, target.position);
    double attractive_force = GRAVITY_CONST * ((reference.mass * target.mass) / (distance * distance));
    return attractive_force;
}

// solves for a in f = ma
extern double calculate_force_acceleration(double attractive_force, struct Body body) {
    double applied_acceleration = 0;
    applied_acceleration = attractive_force / body.mass;
    return applied_acceleration;
}

// Calculates Force vector to apply to the movement vector.
Vector2 caluclate_orbit_vector(struct Body reference, struct Body target)
{
    Vector2 new_velocity_vector = {(double)0, (double)0};
    // data->bodyDistance = distance_to_body(reference.position, target.position);
    double distance = distance_to_body(reference.position, target.position);
    double attractive_force = GRAVITY_CONST * ((reference.mass * target.mass) / powf (distance, 2));
    double acceleration = calculate_force_acceleration(attractive_force, reference);

    Vector2 vector_to_target = Vector2Subtract(target.position, reference.position);
    vector_to_target = Vector2Normalize(vector_to_target);

    Vector2 acceleration_vector = Vector2Scale(vector_to_target, acceleration);
    
    new_velocity_vector = Vector2Add(reference.velocity, acceleration_vector);

    return new_velocity_vector;
}

