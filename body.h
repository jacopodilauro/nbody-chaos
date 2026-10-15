#ifndef BODY_H
#define BODY_H

#include <raylib.h>
#include <vector>
class Body{
    
    public: 
        double mass;
        Vector3 pos;
        Vector3 vel;
        Vector3 acc;
        Color color;
        std::vector<Vector3> trail;

        Body() : mass (1), pos ({0}), 
            vel ({0}), acc ({0}), color (WHITE) {}

        Body(double m, Vector3 p, Vector3 v, Vector3 a, Color c) : 
            mass (m), pos (p), vel (v), acc (a), color (c) {}

        void draw2D(){
            DrawCircle(pos.x, pos.y, 5, color);
        }

        void drawTrail3D(){
            for(int i = 1; i < 100; i++){
                DrawLine3D(trajectory[i], trajectory[i-1], color);
            }
        }

        void draw3D(){
            DrawSphere(pos, 10, color); 
        }

};

#endif
