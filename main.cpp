#include <raylib.h>
#include <iostream>
#include <vector>

#include "body.h"
#include "sim.h"
#define WIDTH 800
#define HEIGHT 450
#define SIZE 5

int main(){
    SetTraceLogLevel(LOG_FATAL);
    InitWindow(WIDTH, HEIGHT, "N Body Simulation");
    std::vector<Body> costellation(SIZE);
    InitBodies(costellation, WIDTH, HEIGHT);

    SetTargetFPS(24);         
    while (!WindowShouldClose())   
    {
        UpdateState(costellation, GetFrameTime());

        BeginDrawing();
               ClearBackground(BLACK);
               DrawBodies(costellation);
               /*BeginMode3D(camera)
               EndMode3D()*/
        EndDrawing();
    }
    CloseWindow();
    return 0;    
}
