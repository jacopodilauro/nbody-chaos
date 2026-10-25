#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#include "args.h"
#define WIDTH 1920 //1920x1080 
#define HEIGHT 1080
#define OFFA -100
#define OFFB 100
#define V_MIN -40
#define V_MAX 40

constexpr float DT = 1.0f / 240.0f;
unsigned int skipFrame = 3;

int main(int argc, char* argv[]){

    Config cfg;
    int ret = ParseArgs(argc, argv, cfg);
    if (ret != -1) return ret;

    auto &[size, seed, g1tilt, g2tilt, is3D, showFPS, useBlend, modeCstl] = cfg;

    SetTraceLogLevel(LOG_FATAL);
    InitWindow(WIDTH, HEIGHT, "N Body Simulation");
    Bodies costellation(size);

    if(modeCstl == "GlxCl") InitGalaxyCollision(costellation, WIDTH, HEIGHT, seed, g1tilt, g2tilt);
    else InitNBodies(costellation, WIDTH, HEIGHT, seed, OFFA, OFFB, V_MIN, V_MAX);
    
    Camera3D camera = { { WIDTH/2.0f - 500, HEIGHT/2.0f + 500.0f, 500.0f },
                        { WIDTH/2, HEIGHT/2, 0/*static_cast<float>(costellation[0].pos.x), static_cast<float>(costellation[0].pos.y), static_cast<float>(costellation[0].pos.z)*/ },            
                        { 0.0f, 1.0f, 0.0f }, 60.0f, CAMERA_PERSPECTIVE };
    
    if (is3D) {
        DisableCursor();
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())   
    {   
        // controls
        if (IsKeyPressed(KEY_SPACE)) {
            cfg.is3D = !cfg.is3D;
            if (cfg.is3D) DisableCursor();
            else EnableCursor();
        }
        if (IsKeyPressed(KEY_B)) useBlend = !useBlend;

        //UpdateState(costellation, GetFrameTime() * 0.25);
        UpdateState(costellation, DT);
        UpdateGalaxyColor(costellation);
        //UpdateTrail(costellation);
        if(cfg.is3D){
            if(skipFrame > 0) skipFrame--;
            else UpdateCamera(&camera, CAMERA_FREE);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            
            if(showFPS) DrawText(std::to_string(GetFPS()).c_str(), 20, 20, 20, WHITE);

            if (!cfg.is3D) {
                if (useBlend) BeginBlendMode(BLEND_ADDITIVE);
                    costellation.draw2D();
                if (useBlend) EndBlendMode();
            } else {
                BeginMode3D(camera);
                    DrawCustomGrid(Vector3{ WIDTH / 2.0f, (HEIGHT / 2.0f) - 150.0f, 0.0f }, 50, 15.0f);
                    if (useBlend) BeginBlendMode(BLEND_ADDITIVE);
                        costellation.draw3D();
                    if (useBlend) EndBlendMode();
                EndMode3D();
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;    
}
