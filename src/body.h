#ifndef BODY_H
#define BODY_H

#include <raylib.h>
#include <vector>

struct Vec3d { double x = 0, y = 0, z = 0; };

class Body{

    public: 
        double m_mass;
        Vec3d m_pos;
        Vec3d m_vel;
        Vec3d m_acc;
        Color m_color;
        std::vector<Vector3> m_trail;

        Body() : m_mass (1), m_pos ({0}), 
            m_vel ({0}), m_acc ({0}), m_color (WHITE) {}

        Body(double m, Vec3d p, Vec3d v, Vec3d a, Color c) : 
            m_mass (m), m_pos (p), m_vel (v), m_acc (a), m_color (c) {}

        void draw2D(){
            DrawCircle(m_pos.x, m_pos.y, 1, m_color);
        }

        void drawTrail2D(){
            for(int i = 1; i < m_trail.size(); i++){
                DrawLine(m_trail[i-1].x, m_trail[i-1].y, m_trail[i].x, m_trail[i].y, m_color); 
            }
        }

        void drawTrail3D(){
            for(int i = 1; i < m_trail.size(); i++){
                DrawLine3D(m_trail[i-1], m_trail[i], m_color); 
            }
        }

        void draw3D(){
            DrawSphere({static_cast<float>(m_pos.x), static_cast<float>(m_pos.y), static_cast<float>(m_pos.z)}, 5, m_color); 
        }

};

#endif
