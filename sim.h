#ifndef SIM_H
#define SIM_H

#include <raylib.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "body.h"

//#define G 6.67e-11
#define G 1000
#define eps 1e-9

void InitBodies(std::vector<Body> &cstl, float WIDTH, float HEIGHT){
    cstl[0].setValues(500, Vector3{WIDTH/2     , HEIGHT/2, 0}, {0, 0, 0}, {0}, WHITE);
    cstl[1].setValues(50, Vector3{WIDTH/2 + 80, HEIGHT/2, 0}, {0, -10, 0}, {0}, RED);
    cstl[2].setValues(50, Vector3{WIDTH/2 - 80, HEIGHT/2, 0}, {0, 10, 0}, {0}, GREEN);
    cstl[3].setValues(50, Vector3{WIDTH/2, HEIGHT/2 + 80, 0}, {0, 10, 0}, {0}, GOLD);
    cstl[4].setValues(50, Vector3{WIDTH/2, HEIGHT/2 - 80, 0}, {0, -10, 0}, {0}, BLUE);
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
