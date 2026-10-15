#ifndef SIM_H
#define SIM_H

#include <raylib.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "body.h"
 
#define G 500 // 6.67e-11
#define eps 25.0f// 1e-9

void InitNBodies(std::vector<Body> &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.size();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distP(pa, pb);
    std::uniform_real_distribution<float> distV(va, vb);
    std::uniform_int_distribution<int> distC(0, 255);
    cstl[0] = Body(1000, Vector3{WIDTH/2     , HEIGHT/2, 0}, {0, 0, 0}, {0}, WHITE);
    for(size_t i = 1; i < size; i++){
        cstl[i] = Body(50, Vector3{WIDTH/2 + distP(gen), HEIGHT/2 + distP(gen), 0}, {distV(gen), distV(gen), 0}, {0}, 
                            Color{static_cast<unsigned char>(distC(gen)), static_cast<unsigned char>(distC(gen)), 
                                  static_cast<unsigned char>(distC(gen)), 255});
    }
}

void InitNBodies3(std::vector<Body> &cstl, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.size();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distP(pa, pb);
    std::uniform_real_distribution<float> distV(va, vb);
    std::uniform_int_distribution<int> distC(0, 255);
    cstl[0] = Body(1000, Vector3{0, 100, 0}, {0, 0, 0}, {0}, WHITE);
    for(size_t i = 1; i < size; i++){
        cstl[i] = Body(50, Vector3{ distP(gen), distP(gen)+50, distP(gen) }, {distV(gen), distV(gen),  distV(gen)}, {0}, 
                            Color{static_cast<unsigned char>(distC(gen)), static_cast<unsigned char>(distC(gen)), 
                                  static_cast<unsigned char>(distC(gen)), 255});
    }
}

void UpdateState(std::vector<Body> &cstl, float dt){
    for (size_t i = 0; i < cstl.size(); ++i) {
        cstl[i].acc = Vector3{0.0f, 0.0f, 0.0f};
    }

    for(size_t i = 0; i < cstl.size(); i++){
        for(size_t j = i+1; j < cstl.size(); j++){
            if(i != j){
                float dx = cstl[j].pos.x - cstl[i].pos.x;
                float dy = cstl[j].pos.y - cstl[i].pos.y;
                float dz = cstl[j].pos.z - cstl[i].pos.z; 

                float distSqr = dx*dx + dy*dy + dz*dz + eps*eps;
                float dist = std::sqrt(distSqr);
                float factor_i = (G * (float)cstl[j].mass) / (distSqr * dist);
                
                cstl[i].acc.x += factor_i * dx;
                cstl[i].acc.y += factor_i * dy;
                cstl[i].acc.z += factor_i * dz;
            
                float factor_j = (G * (float)cstl[i].mass) / (distSqr * dist);
                cstl[j].acc.x -= factor_j * dx;
                cstl[j].acc.y -= factor_j * dy;
                cstl[j].acc.z -= factor_j * dz;
            }
        }
    }
    
    for(size_t i = 0; i < cstl.size(); i++){
        cstl[i].vel.x += cstl[i].acc.x * dt;
        cstl[i].vel.y += cstl[i].acc.y * dt;
        cstl[i].vel.z += cstl[i].acc.z * dt;
        
        cstl[i].pos.x += cstl[i].vel.x * dt;
        cstl[i].pos.y += cstl[i].vel.y * dt;
        cstl[i].pos.z += cstl[i].vel.z * dt;
    }
}

void DrawBodies2(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        body.draw2D();
    }
}

void DrawBodies3(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        body.draw3D();
    }
}

#endif
