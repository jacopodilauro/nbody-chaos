#include <raylib.h>
#include <vector>

#include "body.h"

Bodies::Bodies(size_t n) : count(n), mass(n, 1.0),
        pos_x(n, 0.0), pos_y(n, 0.0), pos_z(n, 0.0),
        vel_x(n, 0.0), vel_y(n, 0.0), vel_z(n, 0.0),
        acc_x(n, 0.0), acc_y(n, 0.0), acc_z(n, 0.0),
        color(n, WHITE),
        trail(n) {}

void Bodies::setBody(size_t i, double m, Vector3 p, Vector3 v, Vector3 a, Color c) {
    mass[i] = m;
    pos_x[i] = p.x; pos_y[i] = p.y; pos_z[i] = p.z;
    vel_x[i] = v.x; vel_y[i] = v.y; vel_z[i] = v.z;
    acc_x[i] = a.x; acc_y[i] = a.y; acc_z[i] = a.z;
    color[i] = c;
}  

void Bodies::drawTrail2D() const {
    for(size_t i = 0; i < trail.size(); i++){    
        const auto& t = trail[i];
        for (size_t j = 1; j < t.size(); j++) {
            DrawLine(static_cast<int>(t[j - 1].x), static_cast<int>(t[j - 1].y), 
                static_cast<int>(t[j].x), static_cast<int>(t[j].y), color[i]);
        }
    }
}

void Bodies::drawTrail3D() const {
    for(size_t i = 0; i < trail.size(); i++){ 
        const auto& t = trail[i];
        for (size_t j = 1; j < t.size(); j++) {
            DrawLine3D(t[j - 1], t[j], color[i]);
        }
    }
}
    
void Bodies::draw2D() const {
        for (size_t i = 0; i < count; i++) {
            //drawTrail2D(trail[i], i);
            //DrawCircle(static_cast<int>(pos_x[i]), static_cast<int>(pos_y[i]), 1.0f, color[i]);
            DrawRectangle(static_cast<int>(pos_x[i]), static_cast<int>(pos_y[i]), 1, 1, color[i]);
            //DrawPixel(static_cast<int>(pos_x[i]), static_cast<int>(pos_y[i]), color[i]);
        }
    }

void Bodies::draw3D() const {
        for (size_t i = 0; i < count; i++) {
            //drawTrail3D(trail[i], i);
            //DrawSphere({pos_x[i], pos_y[i], pos_z[i]}, 1.0f, color[i]);
            DrawCube({pos_x[i], pos_y[i], pos_z[i]}, 1.0f, 1.0f, 1.0f, color[i]);  
            //DrawPoint3D({pos_x[i], pos_y[i], pos_z[i]}, color[i]); 
        }
    }