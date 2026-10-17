#ifndef BODY_H
#define BODY_H

#include <raylib.h>
#include <vector>

struct Vec3d { double x = 0, y = 0, z = 0; };

class Body{

    public: 
        double mass;
        Vec3d pos;
        Vec3d vel;
        Vec3d acc;
        Color color;
        std::vector<Vector3> trail;

        Body() : mass (1), pos ({0}), 
            vel ({0}), acc ({0}), color (WHITE) {}

        Body(double m, Vec3d p, Vec3d v, Vec3d a, Color c) : 
            mass (m), pos (p), vel (v), acc (a), color (c) {}

        void draw2D(){
            DrawCircle(pos.x, pos.y, 5, color);
        }

        void drawTrail2D(){
            for(int i = 1; i < trail.size(); i++){
                DrawLine(trail[i-1].x, trail[i-1].y, trail[i].x, trail[i].y, color); 
            }
        }

        void drawTrail3D(){
            for(int i = 1; i < trail.size(); i++){
                DrawLine3D(trail[i-1], trail[i], color); 
            }
        }

        void draw3D(){
            DrawSphere({static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z)}, 10, color); 
        }

};

#endif
