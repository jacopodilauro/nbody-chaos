#ifndef SIM_H
#define SIM_H

#if defined(_OPENMP) && !defined(NO_SIMD)
  #define PAR_LOOP _Pragma("omp parallel for simd schedule(static)")   // core + SIMD
#elif defined(_OPENMP)
  #define PAR_LOOP _Pragma("omp parallel for schedule(static)")        // solo core
#else
  #define PAR_LOOP _Pragma("omp simd")                                 // solo SIMD (o niente)
#endif

#ifdef _OPENMP
  #define PAR_FOR _Pragma("omp parallel for schedule(static)")
#else
  #define PAR_FOR
#endif

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "body.h"
 
constexpr double G = 500; // 6.67e-11
constexpr double EPS2 = 20.0 * 20.0;
//constexpr int eps = 20;// 1e-9

void InitNBodies(Bodies &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.count;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distP(pa, pb);
    std::uniform_real_distribution<float> distV(va, vb);
    std::uniform_int_distribution<int> distC(0, 255);

    cstl.setBody(0, 500.0, {WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0}, WHITE);
    for(size_t i = 1; i < size; i++){
        cstl.setBody(i, 50, {WIDTH / 2.0f + distP(gen), HEIGHT / 2.0f + distP(gen), distP(gen)},
                            {distV(gen), distV(gen), distV(gen)}, {0}, 
                            Color{static_cast<unsigned char>(distC(gen)), static_cast<unsigned char>(distC(gen)), 
                                  static_cast<unsigned char>(distC(gen)), 255});
    }
}

void UpdateState(Bodies &cstl, float dt){
    const size_t size = cstl.count; 

    const double* __restrict l_m = cstl.mass.data();
    const double* __restrict l_px = cstl.pos_x.data();
    const double* __restrict l_py = cstl.pos_y.data();
    const double* __restrict l_pz = cstl.pos_z.data();

    double* __restrict l_ax = cstl.acc_x.data();
    double* __restrict l_ay = cstl.acc_y.data();
    double* __restrict l_az = cstl.acc_z.data();
    double* __restrict l_vx = cstl.vel_x.data();
    double* __restrict l_vy = cstl.vel_y.data();
    double* __restrict l_vz = cstl.vel_z.data();

    PAR_LOOP
    for (size_t i = 0; i < size; i++) {
        const double xi = l_px[i];
        const double yi = l_py[i];
        const double zi = l_pz[i];
        double ax = 0, ay = 0, az = 0; 

        
        for(size_t j = 0; j < size; j++){

            double dx = l_px[j] - xi;
            double dy = l_py[j] - yi;
            double dz = l_pz[j] - zi; 

            double distSqr = dx*dx + dy*dy + dz*dz + EPS2;
            double dist = std::sqrt(distSqr);
            double factor_i = (G * (double)l_m[j]) / (distSqr * dist);
            
            ax += factor_i * dx;
            ay += factor_i * dy;
            az += factor_i * dz;
        }
        l_ax[i] = ax;
        l_ay[i] = ay;
        l_az[i] = az;
    }
    
    double* __restrict px_next = cstl.pos_x.data();
    double* __restrict py_next = cstl.pos_y.data();
    double* __restrict pz_next = cstl.pos_z.data();
    
    PAR_LOOP
    for (size_t i = 0; i < size; i++) {
        l_vx[i] += l_ax[i] * dt;
        l_vy[i] += l_ay[i] * dt;
        l_vz[i] += l_az[i] * dt;

        px_next[i] += l_vx[i] * dt;
        py_next[i] += l_vy[i] * dt;
        pz_next[i] += l_vz[i] * dt;
    }
}

void UpdateGalaxyColor(Bodies &cstl){
    size_t n = cstl.count;
    size_t m = n / 2; 
    
    PAR_FOR   
    for(size_t i = 0; i < n; i++){
        if (cstl.mass[i] < 1.0) {
            double l_vx = cstl.vel_x[i], l_vy = cstl.vel_y[i], l_vz = cstl.vel_z[i];
            double speed = std::sqrt(l_vx*l_vx + l_vy*l_vy + l_vz*l_vz);
            float s = std::min(1.0f, static_cast<float>(speed / 1300.0));
            if(i < m){
                cstl.color[i] = Color{
                    static_cast<unsigned char>(45 + 210 * s * s),
                    static_cast<unsigned char>(30 + 215 * s),
                    static_cast<unsigned char>(130 + 125 * s),
                    255
                };
            }else{    
                cstl.color[i] = Color{
                    static_cast<unsigned char>(100 + 155 * s),
                    static_cast<unsigned char>(20 + 70 * s * s),
                    static_cast<unsigned char>(140 - 50 * s),
                    255
                };
            }   
        }
    }
}

void UpdateTrail(Bodies &cstl){
    size_t l = cstl.count;
    
    PAR_FOR   
    for(size_t i = 0; i < l; i++){
        cstl.trail[i].push_back({static_cast<float>(cstl.pos_x[i]), static_cast<float>(cstl.pos_y[i]), static_cast<float>(cstl.pos_z[i])});
        if(cstl.trail[i].size() > 25){
            cstl.trail[i].erase(cstl.trail[i].begin());
        }   
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
void InitGalaxyCollision(Bodies &b, float WIDTH, float HEIGHT, int seed, float g1tilt, float g2tilt) {
    size_t size = b.count;
    if (size < 2) return;
    size_t half = size / 2;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distR(25.0f, 220.0f);
    std::normal_distribution<float> noise(0.0f, 6.0f);

    double bhMass = 80000.0;
    float tilt2 = g2tilt * DEG2RAD;
    float tilt1 = g1tilt * DEG2RAD;
    Vector3 axis = { 1.0f, 0.0f, 0.0f };

    Vector3 pos1 = {WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f + 100.0f, 0.0f};
    Vector3 vel1 = {45.0f, -10.0f, 0.0f};
    b.setBody(0, bhMass, pos1, vel1, {0}, WHITE);

    Vector3 pos2 = {WIDTH / 2.0f + 300.0f, HEIGHT / 2.0f - 100.0f, 0.0f};
    Vector3 vel2 = {-45.0f, 10.0f, 0.0f};
    b.setBody(half, bhMass, pos2, vel2, {0}, WHITE);

    for (size_t i = 1; i < size; i++) {
        if (i == half) continue;

        bool isFirst = (i < half);
        Vector3 centerPos = isFirst ? pos1 : pos2;
        Vector3 centerVel = isFirst ? vel1 : vel2;

        float r = distR(gen);
        float angle = (i % 2) * PI + 2.0f * std::log(r) + (noise(gen) * 0.06f);

        Vector3 offPos = { r * std::cos(angle), r * std::sin(angle), noise(gen) };

        float dist = std::sqrt(offPos.x * offPos.x + offPos.y * offPos.y);
        float v = std::sqrt((G * bhMass) / dist);
        Vector3 offVel = { -v * (offPos.y / dist), v * (offPos.x / dist), 0.0f };

        offPos = Vector3RotateByAxisAngle(offPos, axis, isFirst ? tilt1 : tilt2);
        offVel = Vector3RotateByAxisAngle(offVel, axis, isFirst ? tilt1 : tilt2);

        Vector3 p  = Vector3Add(centerPos, offPos);
        Vector3 vv = Vector3Add(centerVel, offVel);

        b.setBody(i, 0.01, p, vv, {0}, WHITE);
    }
}

#endif
