#include "body.h"

extern float distance_to_body(Vector2 reference, Vector2 target)
{
    float distance;
    float dist_x = (float)pow((reference.x - target.x), 2);
    float dist_y = (float)pow((reference.y - target.y), 2);
    distance = sqrtf(dist_x + dist_y);

    return distance;
}

extern float caluclate_attractive_force(struct Body reference, struct Body target) {
    float distance = distance_to_body(reference.position, target.position);
    float attractive_force = GRAVITY_CONST * ((reference.mass * target.mass) / (distance * distance));
    return attractive_force;
}

// solves for a in f = ma
extern float calculate_force_acceleration(float attractive_force, struct Body body) {
    float applied_acceleration = 0;
    applied_acceleration = attractive_force / body.mass;
    return applied_acceleration;
}

// Calculates Force vector to apply to the movement vector.
Vector2 caluclate_orbit_vector(struct Body reference, struct Body target)
{
    Vector2 new_velocity_vector = {(float)0, (float)0};
    // data->bodyDistance = distance_to_body(reference.position, target.position);
    float distance = distance_to_body(reference.position, target.position);
    float attractive_force = GRAVITY_CONST * ((reference.mass * target.mass) / powf (distance, 2));
    float acceleration = calculate_force_acceleration(attractive_force, reference);

    Vector2 vector_to_target = Vector2Subtract(target.position, reference.position);
    vector_to_target = Vector2Normalize(vector_to_target);

    Vector2 acceleration_vector = Vector2Scale(vector_to_target, acceleration);
    
    new_velocity_vector = Vector2Add(reference.velocity, acceleration_vector);

    return new_velocity_vector;
}

