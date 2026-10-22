#ifndef SIM_H
#define SIM_H

#ifdef _OPENMP
#include <omp.h>
#endif

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "body.h"
 
#define G 500 // 6.67e-11
#define eps 20// 1e-9

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

void UpdateState(std::vector<Body> &cstl, float dt){

    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (size_t i = 0; i < cstl.size(); ++i) {
        cstl[i].acc = Vec3d{0.0f, 0.0f, 0.0f};
    }

    #ifdef _OPENMP
    #pragma omp parallel for //schedule(dynamic)
    #endif
    for(size_t i = 0; i < cstl.size(); i++){
        for(size_t j = 0; j < cstl.size(); j++){
            if (cstl[j].mass < 1.0) continue;
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
    }
    
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for(size_t i = 0; i < cstl.size(); i++){
        cstl[i].vel.x += cstl[i].acc.x * dt;
        cstl[i].vel.y += cstl[i].acc.y * dt;
        cstl[i].vel.z += cstl[i].acc.z * dt;
        
        cstl[i].pos.x += cstl[i].vel.x * dt;
        cstl[i].pos.y += cstl[i].vel.y * dt;
        cstl[i].pos.z += cstl[i].vel.z * dt;
    }
}

void UpdateGalaxyColor(std::vector<Body> &cstl){
    size_t m = cstl.size() / 2; 
    
    for(size_t i = 0; i < cstl.size(); i++){
        if (cstl[i].mass < 1.0) {
            double speed = std::sqrt(cstl[i].vel.x*cstl[i].vel.x + cstl[i].vel.y*cstl[i].vel.y + cstl[i].vel.z*cstl[i].vel.z);
            float s = std::min(1.0f, static_cast<float>(speed / 1300.0));
            if(i < m){
                cstl[i].color = Color{
                    static_cast<unsigned char>(45 + 210 * s * s),
                    static_cast<unsigned char>(30 + 215 * s),
                    static_cast<unsigned char>(130 + 125 * s),
                    255
                };
            }else{    
                cstl[i].color = Color{
                    static_cast<unsigned char>(100 + 155 * s),
                    static_cast<unsigned char>(20 + 70 * s * s),
                    static_cast<unsigned char>(140 - 50 * s),
                    255
                };
            }   
        }
    }
}

void UpdateTrail(std::vector<Body> &cstl){
    size_t l = cstl.size();
    for(size_t i = 0; i < l; i++){
        cstl[i].trail.push_back({static_cast<float>(cstl[i].pos.x), static_cast<float>(cstl[i].pos.y), static_cast<float>(cstl[i].pos.z)});
        if(cstl[i].trail.size() > 25){
            cstl[i].trail.erase(cstl[i].trail.begin());
        }   
    }
}

void DrawBodies2(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        //body.drawTrail2D();
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

// Galaxy collision
void InitGalaxyCollision(std::vector<Body> &cstl, float WIDTH, float HEIGHT, int seed, float g1tilt, float g2tilt) {
    size_t size = cstl.size();
    if (size < 2) return;
    size_t half = size / 2;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distR(25.0f, 220.0f);
    std::normal_distribution<float> noise(0.0f, 6.0f);

    double bhMass = 80000.0;

    float tilt2 = g2tilt * DEG2RAD;
    float tilt1 = g1tilt * DEG2RAD;
    Vector3 axis = { 1.0f, 0.0f, 0.0f };

    Vec3d pos1 = {WIDTH / 2.0 - 300.0, HEIGHT / 2.0 + 100.0, 0.0};
    Vec3d vel1 = {45.0, -10.0, 0.0};
    cstl[0] = Body(bhMass, pos1, vel1, {0,0,0}, WHITE);

    Vec3d pos2 = {WIDTH / 2.0 + 300.0, HEIGHT / 2.0 - 100.0, 0.0};
    Vec3d vel2 = {-45.0, 10.0, 0.0};
    cstl[half] = Body(bhMass, pos2, vel2, {0,0,0}, WHITE);

    for (size_t i = 1; i < size; i++) {
        if (i == half) continue;

        bool isFirst = (i < half);
        Vec3d centerPos = isFirst ? pos1 : pos2;
        Vec3d centerVel = isFirst ? vel1 : vel2;

        float r = distR(gen);
        float angle = (i % 2) * PI + 2.0f * std::log(r) + (noise(gen) * 0.06f);

        Vector3 offPos = { r * std::cos(angle), r * std::sin(angle), noise(gen) };

        float dist = std::sqrt(offPos.x * offPos.x + offPos.y * offPos.y);
        float v = std::sqrt((G * bhMass) / dist);
        Vector3 offVel = { -v * (offPos.y / dist), v * (offPos.x / dist), 0.0f };

        if (!isFirst) {
            offPos = Vector3RotateByAxisAngle(offPos, axis, tilt2);
            offVel = Vector3RotateByAxisAngle(offVel, axis, tilt2);
        }else{
            offPos = Vector3RotateByAxisAngle(offPos, axis, tilt1);
            offVel = Vector3RotateByAxisAngle(offVel, axis, tilt1);
        }

        Vec3d p  = { centerPos.x + offPos.x, centerPos.y + offPos.y, centerPos.z + offPos.z };
        Vec3d vv = { centerVel.x + offVel.x, centerVel.y + offVel.y, centerVel.z + offVel.z };

        cstl[i] = Body(0.01, p, vv, {0,0,0}, WHITE);
    }
}
#endif
