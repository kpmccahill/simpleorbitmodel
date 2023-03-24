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
    float bodyDistance;
    float attractiveForce; // newtons universal grav
};

struct OrbitLine {
    int max_index;
    int current_index; // current index for insertion
    Vector2 orbitalPoints[256];
};

void add_orbit_point(struct OrbitLine *orbitLine, Vector2 point) {
    if (orbitLine->current_index + 1 >= orbitLine->max_index) {
        orbitLine -> current_index = 0;
    }

    orbitLine->orbitalPoints[orbitLine->current_index] = point;
}

void draw_orbit_line(Vector2 *orbitalPoints[]) { 

}

// Program main entry point
int main(void)
{
    // Initialization
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib simple orbital sim");

    // Camera -- function
    Camera2D camera = { 0 };
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight /2.0f}; // sets the camera to move from its center
    camera.target = (Vector2){0, 0}; // centers the camea over the star
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;

    float cameraSpeed = 7.0f;
    float camera_zoom_max = 5.0f;
    float camera_zoom_min = 0.1f;
    
    // Star -- function
    Vector2 starPosition = { 0, 0 };
    Vector2 starVelocity = { (float) 0, (float)0 };
    float starMass = 333000000;
    float starRadius = 64;
    struct Body star = {starPosition, starVelocity, starMass, starRadius};

    // Orbiting Body -- function
    Vector2 bodyPosition = { 150, 0 };
    // Vector2 bodyVelocity = { (float)-0.00, (float)-0.003 }; // avg earth 29,784.8 m/s
    Vector2 bodyVelocity = { (float)-0.00, (float)-0.03 }; // avg earth 29,784.8 m/s
    float bodyMass = 1;
    float bodyRadius = 5;
    struct Body body = {bodyPosition, bodyVelocity, bodyMass, bodyRadius};

    struct OrbitData data = { distance_to_body(body.position, star.position) };


    // Important stuff
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update

        // Camera Movement
        if (IsKeyDown(KEY_A)) camera.target.x -= cameraSpeed;
        else if (IsKeyDown(KEY_D)) camera.target.x += cameraSpeed;

        if (IsKeyDown(KEY_W)) camera.target.y -= cameraSpeed;
        else if (IsKeyDown(KEY_S)) camera.target.y += cameraSpeed;


        // camera zoom
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (camera.zoom > camera_zoom_max) camera.zoom = camera_zoom_max;
        else if (camera.zoom < camera_zoom_min) camera.zoom = camera_zoom_min;

        if (IsKeyPressed(KEY_R)) {
            camera.zoom = 1.0f;
        }

        // Body movement
        body.velocity = caluclate_orbit_vector(body, star);

        body.position.y += body.velocity.y;
        body.position.x += body.velocity.x;


        // data update
        data.bodyDistance = distance_to_body(body.position, star.position);
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // world sorta
            BeginMode2D(camera);
                DrawCircleV(star.position, star.radius, YELLOW); // truescale 695
                DrawCircleV(body.position, body.radius, BLUE); // truescale 6
                DrawLineV(body.position, Vector2Add(body.position, Vector2Scale(body.velocity, 3000)), RED); // draws a line showing the orbit.
            EndMode2D();
            
            // DrawText(TextFormat("Star Mass: %.10e", star.mass), 20, 50, 28, BLACK);
            // DrawText(TextFormat("Body Mass: %.10e", body.mass), 20, 70, 28, BLACK);
            DrawText(TextFormat("Star Mass: %.2f", star.mass), 20, 50, 28, BLACK);
            DrawText(TextFormat("Body Mass: %.2f", body.mass), 20, 70, 28, BLACK);
            DrawText(TextFormat("Body Distance: %.3f", data.bodyDistance), 20, 90, 28, BLACK);
            DrawText(TextFormat("Body Velocity X: %.8f", body.velocity.x), 20, 110, 28, BLACK);
            DrawText(TextFormat("Body Velocity Y: %.8f", body.velocity.y), 20, 130, 28, BLACK);


        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    return 0;
}
