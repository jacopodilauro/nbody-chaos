#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#define WIDTH 1920 //1920x1080
#define HEIGHT 1000
#define OFFA -100
#define OFFB 100
#define V_MIN -40
#define V_MAX 40

int main(int argc, char* argv[]){
    uint size = 3;
    uint seed = 4;
    uint skipFrame = 3;

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
    //InitNBodies(costellation, WIDTH, HEIGHT, seed, OFFA, OFFB, V_MIN, V_MAX);
    InitGalaxyCollision(costellation, WIDTH, HEIGHT, seed);
    Camera3D camera = { { WIDTH/2.0f, HEIGHT/2.0f + 200.0f, 700.0f },
                        { WIDTH/2, HEIGHT/2, 0/*static_cast<float>(costellation[0].pos.x), static_cast<float>(costellation[0].pos.y), static_cast<float>(costellation[0].pos.z)*/ },            
                        { 0.0f, 1.0f, 0.0f }, 60.0f, CAMERA_PERSPECTIVE };

    
    if (is3D) {
        DisableCursor();
    }
    

    SetTargetFPS(60);
    while (!WindowShouldClose())   
    {   
        if (IsKeyPressed(KEY_SPACE)) {
            is3D = !is3D;
            if (is3D) {
                DisableCursor();
            }
            else EnableCursor();
        }

        UpdateState(costellation, GetFrameTime() * 0.25f);
        //UpdateTrail(costellation);
        if(is3D){
            if(skipFrame > 0) skipFrame--;
            else UpdateCamera(&camera, CAMERA_FREE);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(std::to_string(GetFPS()).c_str(), 20, 20, 20, WHITE);

            if (!is3D) {
                DrawBodies2(costellation);
            } else {
                BeginMode3D(camera);
                    DrawCustomGrid(Vector3{ WIDTH / 2.0f, (HEIGHT / 2.0f) - 150.0f, 0.0f }, 50, 15.0f);
                    DrawBodies3(costellation);
                EndMode3D();
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;    
}
