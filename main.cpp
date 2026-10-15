#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#define WIDTH 1200
#define HEIGHT 700
#define OFFA -100
#define OFFB 100
#define V_MIN -40
#define V_MAX 40

int main(int argc, char* argv[]){
    uint size = 3;
    uint seed = 4;
    bool is3D = false;
    if(argc > 3){
        std::cout << "Use <number of body> <mode>" << std::endl;
        return 1;
    }else if(argc >= 2)
        size = std::stoi(argv[1]);
    if (argc == 3) 
        is3D = std::stoi(argv[2]);
    

    SetTraceLogLevel(LOG_FATAL);
    InitWindow(WIDTH, HEIGHT, "N Body Simulation");
    std::vector<Body> costellation(size);
    Camera3D camera = {{ -200.0f, 200.0f, 200.0f }, { 0.0f, 100.0f, 0.0f },
                        { 0.0f, 1.0f, 0.0f }, 60.0f, CAMERA_PERSPECTIVE};

    if (!is3D) {
        InitNBodies(costellation, WIDTH, HEIGHT, seed, OFFA, OFFB, V_MIN, V_MAX);
    } else {
        InitNBodies3(costellation, seed, OFFA, OFFB, V_MIN, V_MAX);
        DisableCursor();
    }
    SetTargetFPS(60);         
    while (!WindowShouldClose())   
    {
        DrawText(std::to_string(GetFPS()).c_str(), 20, 20, 20, WHITE);
        if (IsKeyPressed(KEY_SPACE)) {
            is3D = !is3D;
            if (is3D) DisableCursor();
            else EnableCursor();
        }

        UpdateState(costellation, GetFrameTime());
        if(is3D) UpdateCamera(&camera, CAMERA_FREE);

        BeginDrawing();
            ClearBackground(BLACK);
            if (!is3D) {
                DrawBodies2(costellation);
            } else {
                BeginMode3D(camera);
                    DrawGrid(50, 10);
                    DrawBodies3(costellation);
                EndMode3D();
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;    
}
