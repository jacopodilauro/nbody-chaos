#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#define WIDTH 800
#define HEIGHT 450
#define OFFA -100
#define OFFB 100
#define VX 10
#define VY 10

int main(int argc, char* argv[]){
    int size = 3;
    int seed = 4;
    if(argc > 3){
        std::cout << "Use <number of body>" << std::endl;
        return 1;
    }else if(argc == 2)
        size = std::stoi(argv[1]);
    

    SetTraceLogLevel(LOG_FATAL);
    InitWindow(WIDTH, HEIGHT, "N Body Simulation");
    std::vector<Body> costellation(size);
//    InitBodies(costellation, WIDTH, HEIGHT);
    InitNBodies(costellation, WIDTH, HEIGHT, seed, OFFA, OFFB, VX, VY);

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
