#ifndef SIM_H
#define SIM_H

#include <raylib.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "body.h"

//#define G 6.67e-11
#define G 500
#define eps 5//e-9

void InitBodies(std::vector<Body> &cstl, float WIDTH, float HEIGHT){
    cstl[0].setValues(50, Vector3{WIDTH/2     , HEIGHT/2, 0}, {4, 4, 0}, {0}, WHITE);
    cstl[1].setValues(50, Vector3{WIDTH/2 + 80, HEIGHT/2, 0}, {0, -10, 0}, {0}, RED);
    cstl[2].setValues(50, Vector3{WIDTH/2 - 80, HEIGHT/2, 0}, {0, 10, 0}, {0}, GREEN);
    cstl[3].setValues(50, Vector3{WIDTH/2, HEIGHT/2 + 80, 0}, {0, 10, 0}, {0}, GOLD);
    cstl[4].setValues(50, Vector3{WIDTH/2, HEIGHT/2 - 80, 0}, {0, -10, 0}, {0}, BLUE);
}

void InitNBodies(std::vector<Body> &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.size();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distP(pa, pb);
    std::uniform_real_distribution<float> distV(va, vb);
    std::uniform_int_distribution<int> distC(0, 255);
    cstl[0].setValues(100, Vector3{WIDTH/2     , HEIGHT/2, 0}, {0, 0, 0}, {0}, WHITE);
    for(size_t i = 1; i < size; i++){
        cstl[i].setValues(50, Vector3{WIDTH/2 + distP(gen), HEIGHT/2 + distP(gen), 0}, {distV(gen), distV(gen), 0}, {0}, 
                            Color{static_cast<unsigned char>(distC(gen)), static_cast<unsigned char>(distC(gen)), 
                                  static_cast<unsigned char>(distC(gen)), 255});
    }
}

void UpdateState(std::vector<Body> &cstl, float dt){
    for(size_t i = 0; i < cstl.size(); i++){
        cstl[i].acc = Vector3{0, 0, 0};

        for(size_t j = 0; j < cstl.size(); j++){
            if(i != j){
                float dx = cstl[j].pos.x - cstl[i].pos.x;
                float dy = cstl[j].pos.y - cstl[i].pos.y;
                float dz = cstl[j].pos.z - cstl[i].pos.z; 

                float distSqr = dx*dx + dy*dy + dz*dz + eps*eps;
                float dist = std::sqrt(distSqr);
                float factor = (G * (float)cstl[j].mass) / (distSqr * dist);
                
                cstl[i].acc.x += factor * dx;
                cstl[i].acc.y += factor * dy;
                cstl[i].acc.z += factor * dz;
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

void DrawBodies(std::vector<Body> &cstl){
    for (Body& body : cstl) {
        body.draw2D();
    }
}

#endif
