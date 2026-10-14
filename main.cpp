#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#define WIDTH 800
#define HEIGHT 450
#define OFFA -200
#define OFFB 200
#define V_MIN -40
#define V_MAX 40

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
    InitNBodies(costellation, WIDTH, HEIGHT, seed, OFFA, OFFB, V_MIN, V_MAX);

    SetTargetFPS(60);         
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
