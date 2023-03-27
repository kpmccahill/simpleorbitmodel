/*******************************************************************************************
*   This project was originally created from the example below:
*   raylib [core] example - Keyboard input
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
*   This project was modified 2023 by Kyle McCahill (@kpmccahill) in order to create this 
*   orbital simulation.
********************************************************************************************/
/// this needs delta time, so maybe time to implement it in godot? or you could just implement delta time in this :) 
/// might as well do that to learn about it why not
#include <math.h>

#include "raylib.h"

#include "body.h"

struct OrbitData
{
    double bodyDistance;
    // double attractiveForce; // newtons universal grav
    Vector2 scaledPosition;
};

struct OrbitLine {
    int max_index;
    int current_index; // current index for insertion
    Vector2 orbital_points[4096]; // could maybe tie a position var ptr to the body
};

void add_orbit_point(struct OrbitLine *orbitLine, Vector2 point) {
    if (orbitLine->current_index + 1 >= orbitLine->max_index) {
        orbitLine -> current_index = 0;
    }

    orbitLine->orbital_points[orbitLine->current_index] = point;
    orbitLine->current_index++;
}

void draw_orbit_line(struct OrbitLine orbitLine) { 
    size_t number_of_points = sizeof(orbitLine.orbital_points)/sizeof(orbitLine.orbital_points[0]);
    DrawLineStrip(orbitLine.orbital_points, (int)number_of_points, RED);
    // for (int i = 0; i < orbitLine.max_index; i++) {
    //     DrawPixelV(orbitLine.orbital_points[i], RED);
    // }
}

// Program main entry point
int main(void)
{
    // Initialization
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    
    const double simScale = 10000000; // from some example of one au in screen scale size, for rendering.
    const Vector2 simScaleVector = {simScale, simScale};

    InitWindow(screenWidth, screenHeight, "raylib simple orbital sim");

    // Camera -- function
    Camera2D camera = { 0 };
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight /2.0f}; // sets the camera to move from its center
    camera.target = (Vector2){0, 0}; // centers the camea over the star
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;

    float cameraSpeed = 5000.0f;
    float camera_zoom_max = 5.0f;
    float camera_zoom_min = 0.1f;
    
    // Star -- function
    Vector2 starPosition = { 0, 0 };
    Vector2 starVelocity = { (double) 0, (double)0 };
    double starMass = 1.9891 * powf(10, 30);  // actual sun mass
    double starRadius = 64;
    struct Body star = {starPosition, starVelocity, starMass, starRadius};

    // Orbiting Body -- function
    Vector2 bodyPosition = { 150000000000, 0 };
    Vector2 bodyVelocity = { (double)-0.00, (double)-29784.0 }; // avg earth 29,784.8 m/s
    double bodyMass = 5.9722 * powf(10, 24);  // actual earth mass
    double bodyRadius = 15;
    struct Body body = {bodyPosition, bodyVelocity, bodyMass, bodyRadius};

    Vector2 scaledPosition = Vector2Divide(body.position, simScaleVector);
    struct OrbitData data = { distance_to_body(body.position, star.position), scaledPosition };

    struct OrbitLine orbit_line = {4096, 0};

    // Important stuff
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        float delta = GetFrameTime(); // for somoother movement

        // Camera Movement
        if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed * delta;
        else if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed * delta;

        if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed * delta;
        else if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed * delta;

        // camera zoom
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (camera.zoom > camera_zoom_max) camera.zoom = camera_zoom_max;
        else if (camera.zoom < camera_zoom_min) camera.zoom = camera_zoom_min;

        if (IsKeyPressed(KEY_R)) {
            camera.zoom = 1.0f;
        }

        // Body movement
        body.velocity = caluclate_orbit_vector(body, star);

        // body.position = Vector2Scale(Vector2Add(body.position, body.velocity), delta);
        body.position.y = body.position.y + body.velocity.y * delta;
        body.position.x = body.position.x + body.velocity.x * delta;


        // data update
        data.bodyDistance = distance_to_body(body.position, star.position);
        data.scaledPosition = Vector2Divide(body.position, simScaleVector);

        // orbit line update
        bodyPosition = body.position;
        if ((int)bodyPosition.y % 5 == 0) add_orbit_point(&orbit_line, data.scaledPosition);
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw simulated bodies
            BeginMode2D(camera);
                DrawCircleV(star.position, star.radius, YELLOW); // truescale 695
                DrawCircleV(data.scaledPosition, body.radius, BLUE); // truescale 6
                DrawLineV(body.position, Vector2Add(body.position, Vector2Scale(body.velocity, 200)), RED); // draws a line showing velocity vector of body
                draw_orbit_line(orbit_line);
            EndMode2D();
            

            // UI --- Orbital Information
            DrawText(TextFormat("Star Mass: %.4e kg", star.mass), 20, 50, 28, BLACK);
            DrawText(TextFormat("Body Mass: %.4e kg", body.mass), 20, 80, 28, BLACK);
            DrawText(TextFormat("Body Distance: %.3f", data.bodyDistance), 20, 110, 28, BLACK);
            DrawText(TextFormat("Body Velocity X: %.8f", body.velocity.x), 20, 140, 28, BLACK);
            DrawText(TextFormat("Body Velocity Y: %.8f", body.velocity.y), 20, 170, 28, BLACK);

            // UI --- Camera Information
            DrawText(TextFormat("Camera Zoom: %.1f", camera.zoom), 20, 970, 28, BLACK);
            DrawText(TextFormat("Camera Position: %.1f, %.1f", camera.target.x, camera.target.y), 20, 1000, 28, BLACK);

            // UI -- Simulation Information.
            DrawText(TextFormat("Time Scale: %.1fx", 1.0), 1600, 1000, 28, BLACK);


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return 0;
}
