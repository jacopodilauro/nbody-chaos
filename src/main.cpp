#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

#include "body.h"
#include "sim.h"
#define WIDTH 1920 //1920x1080 
#define HEIGHT 1080
#define OFFA -100
#define OFFB 100
#define V_MIN -40
#define V_MAX 40

constexpr float DT = 1.0f / 240.0f;

int main(int argc, char* argv[]){
    unsigned int size = 3;
    unsigned int seed = 4;
    unsigned int skipFrame = 3;
    float g1tilt = 0;
    float g2tilt = 0;
    bool is3D = false;
    bool sizetmp = false;
    bool showFPS = false;
    bool useBlend = false;
    std::string modeCstl = "default";

    // comands
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-3D") {
            is3D = true;
        } else if (arg == "-GlxCl") {
            modeCstl = "GlxCl";
        }else if (arg == "-fps") {
            showFPS = true;
        }else if (arg == "-blend") {
            useBlend = true;
        }else if (arg == "--seed") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --seed\n";
                return 1;
            }
            try {
                seed = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid seed\n";
                return 1;
            }
        }else if (arg == "--g1tilt") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --g1tilt\n";
                return 1;
            }
            try {
                g1tilt = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        }else if (arg == "--g2tilt") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --g2tilt\n";
                return 1;
            }
            try {
                g2tilt = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "Invalid inclination\n";
                return 1;
            }
        }else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: nbody [NUMBER_OF_BODIES] [OPTIONS]\n\n";
            std::cout << "Arguments:\n";
            std::cout << "  NUMBER_OF_BODIES     Number of celestial bodies (optional, default: 3)\n\n";
            std::cout << "Options:\n";
            std::cout << "  -3D                  Enable 3D visualization\n";
            std::cout << "  -GlxCl               Enable galaxy collision mode\n";
            std::cout << "  -fps                 Show FPS on screen\n";
            std::cout << "  -blend               Enable additive blending mode\n";
            std::cout << "  --g1tilt <DEG>       Tilt angle for the first galaxy (in degrees)\n";
            std::cout << "  --g2tilt <DEG>       Tilt angle for the second galaxy (in degrees)\n";
            std::cout << "  --seed <N>           Set the seed for random generation\n";
            std::cout << "  -h, --help           Show this help message\n";
            return 0;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown flag: " << arg << '\n';
            return 1;
        } else if (!sizetmp) {
            try {
                size = std::stoi(arg);
                if (size < 1) { 
                    std::cerr << "Number of bodies must be at least 1\n"; 
                    return 1; 
                }
                sizetmp = true;
            } catch (...) {
                std::cerr << "Invalid number\n";
                return 1;
            }
        } else {
            std::cerr << "Unexpected argument: " << arg << '\n';
            return 1;
        }
    }

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
            is3D = !is3D;
            if (is3D) DisableCursor();
            else EnableCursor();
        }
        if (IsKeyPressed(KEY_B)) useBlend = !useBlend;

        UpdateState(costellation, DT);
        UpdateGalaxyColor(costellation);
        //UpdateTrail(costellation);
        if(is3D){
            if(skipFrame > 0) skipFrame--;
            else UpdateCamera(&camera, CAMERA_FREE);
        }

        BeginDrawing();
            ClearBackground(BLACK);
            
            if(showFPS) DrawText(std::to_string(GetFPS()).c_str(), 20, 20, 20, WHITE);

            if (!is3D) {
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
