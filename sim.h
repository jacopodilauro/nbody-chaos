#ifndef SIM_H
#define SIM_H

#include <raylib.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "body.h"
 
#define G 500 // 6.67e-11
#define eps 20// 1e-9

/**
 * 1/4PiESp * q+ * q- /r^2
 */

void InitNBodies(std::vector<Body> &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.size();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distP(pa, pb);
    std::uniform_real_distribution<float> distV(va, vb);
    std::uniform_int_distribution<int> distC(0, 255);
    cstl[0] = Body(500, Vec3d{WIDTH/2     , HEIGHT/2, 0}, {0, 0, 0}, {0}, WHITE);
    for(size_t i = 1; i < size; i++){
        cstl[i] = Body(50, Vec3d{WIDTH/2 + distP(gen), HEIGHT/2 + distP(gen), distP(gen)}, {distV(gen), distV(gen), distV(gen)}, {0}, 
                            Color{static_cast<unsigned char>(distC(gen)), static_cast<unsigned char>(distC(gen)), 
                                  static_cast<unsigned char>(distC(gen)), 255});
    }
}

void InitFigure8(std::vector<Body> &cstl){
    const double k = std::sqrt((double)G);   // sqrt(G * m), m = 1
    cstl[0] = Body(1, Vec3d{-0.97000436,  0.24308753, 0}, Vec3d{ 0.466203685*k,  0.43236573*k, 0}, Vec3d{}, ORANGE);
    cstl[1] = Body(1, Vec3d{ 0.97000436, -0.24308753, 0}, Vec3d{ 0.466203685*k,  0.43236573*k, 0}, Vec3d{}, WHITE);
    cstl[2] = Body(1, Vec3d{ 0, 0, 0 },                   Vec3d{-0.93240737*k,  -0.86473146*k, 0}, Vec3d{}, PURPLE);
}

void UpdateState(std::vector<Body> &cstl, float dt){
    for (size_t i = 0; i < cstl.size(); ++i) {
        cstl[i].acc = Vec3d{0.0f, 0.0f, 0.0f};
    }

    for(size_t i = 0; i < cstl.size(); i++){
        for(size_t j = 0; j < cstl.size(); j++){
            if(i != j){
                double dx = cstl[j].pos.x - cstl[i].pos.x;
                double dy = cstl[j].pos.y - cstl[i].pos.y;
                double dz = cstl[j].pos.z - cstl[i].pos.z; 

                double distSqr = dx*dx + dy*dy + dz*dz + eps*eps;
                double dist = std::sqrt(distSqr);
                double factor_i = (G * (double)cstl[j].mass) / (distSqr * dist);
                
                cstl[i].acc.x += factor_i * dx;
                cstl[i].acc.y += factor_i * dy;
                cstl[i].acc.z += factor_i * dz;
            }
        }
    } // G * m_1 * m_2 / r^2, F = ma, v(t+dt) = v(t) * a, p(t+dt) = p(t) + v(t)*a^2(t)
    
    for(size_t i = 0; i < cstl.size(); i++){
        cstl[i].vel.x += cstl[i].acc.x * dt;
        cstl[i].vel.y += cstl[i].acc.y * dt;
        cstl[i].vel.z += cstl[i].acc.z * dt;
        
        cstl[i].pos.x += cstl[i].vel.x * dt;
        cstl[i].pos.y += cstl[i].vel.y * dt;
        cstl[i].pos.z += cstl[i].vel.z * dt;
    }
}

void UpdateTrail(std::vector<Body> &cstl){
    for(size_t i = 0; i < cstl.size(); i++){
        cstl[i].trail.push_back({static_cast<float>(cstl[i].pos.x), static_cast<float>(cstl[i].pos.y), static_cast<float>(cstl[i].pos.z)});
    }
}

void DrawBodies2(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        body.drawTrail2D();
        body.draw2D();
    }
}

void DrawBodies3(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        body.drawTrail3D();
        body.draw3D();
    }
}

void DrawCustomGrid(Vector3 center, int slices, float spacing) {
    int halfSlices = slices / 2;
    float halfSize = halfSlices * spacing;

    for (int i = -halfSlices; i <= halfSlices; i++) {
        Color lineColor = (i == 0) ? LIGHTGRAY : DARKGRAY;
        float offset = i * spacing;
        DrawLine3D( Vector3{ center.x - halfSize, center.y, center.z + offset },
                    Vector3{ center.x + halfSize, center.y, center.z + offset },
                    lineColor );
        DrawLine3D( Vector3{ center.x + offset, center.y, center.z - halfSize },
                    Vector3{ center.x + offset, center.y, center.z + halfSize },
                    lineColor );
    }
}

#endif
