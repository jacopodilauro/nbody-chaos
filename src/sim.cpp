
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <random>              
#include <vector>
#include <cmath>

#include "../include/sim.h"

constexpr float G = 500; 
constexpr float EPS2 = 20.0 * 20.0;

void InitNBodies(Bodies &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb){
    size_t size = cstl.count;
    cstl.opt_importantcount = size;
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
    const size_t opt_size = cstl.opt_importantcount; 
    const size_t size = cstl.count;

    const float* __restrict l_m = cstl.mass.data();
    float* __restrict l_px = cstl.pos_x.data();
    float* __restrict l_py = cstl.pos_y.data();
    float* __restrict l_pz = cstl.pos_z.data();

    float* __restrict l_ax = cstl.acc_x.data();
    float* __restrict l_ay = cstl.acc_y.data();
    float* __restrict l_az = cstl.acc_z.data();
    float* __restrict l_vx = cstl.vel_x.data();
    float* __restrict l_vy = cstl.vel_y.data();
    float* __restrict l_vz = cstl.vel_z.data();

    OMP_PARALLEL_FOR
    for (size_t i = 0; i < size; i++) {
        const float xi = l_px[i];
        const float yi = l_py[i];
        const float zi = l_pz[i];
        float ax = 0, ay = 0, az = 0; 

        #pragma omp simd reduction(+:ax, ay, az)
        for(size_t j = 0; j < opt_size; j++){

            float dx = l_px[j] - xi;
            float dy = l_py[j] - yi;
            float dz = l_pz[j] - zi; 

            float distSqr = dx*dx + dy*dy + dz*dz + EPS2;
            float dist = std::sqrt(distSqr);
            float factor_i = (G * (float)l_m[j]) / (distSqr * dist);
            
            ax += factor_i * dx;
            ay += factor_i * dy;
            az += factor_i * dz;
        }
        l_ax[i] = ax;
        l_ay[i] = ay;
        l_az[i] = az;
    }

    OMP_PARALLEL_FOR_SIMD
    for (size_t i = 0; i < size; i++) {
        l_vx[i] += l_ax[i] * dt;
        l_vy[i] += l_ay[i] * dt;
        l_vz[i] += l_az[i] * dt;

        l_px[i] += l_vx[i] * dt;
        l_py[i] += l_vy[i] * dt;
        l_pz[i] += l_vz[i] * dt;
    }
}

void UpdateGalaxyColor(Bodies &cstl){
    size_t n = cstl.count;
    size_t m = n / 2; 
    
    OMP_PARALLEL_FOR   
    for(size_t i = 0; i < n; i++){
        if (cstl.mass[i] < 1.0) {
            float l_vx = cstl.vel_x[i], l_vy = cstl.vel_y[i], l_vz = cstl.vel_z[i];
            float speed = std::sqrt(l_vx*l_vx + l_vy*l_vy + l_vz*l_vz);
            float s = std::min(1.0f, static_cast<float>(speed / 1200.0));
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
    
    OMP_PARALLEL_FOR   
    for(size_t i = 0; i < l; i++){
        cstl.trail[i].push_back({cstl.pos_x[i], cstl.pos_y[i], cstl.pos_z[i]});
        if(cstl.trail[i].size() > 10){
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

void InitGalaxyCollision(Bodies &cstl, float WIDTH, float HEIGHT, int seed, unsigned int opt_mode, float g1tilt, float g2tilt) {
    size_t size = cstl.count;
    if (size < 2) return;
    cstl.opt_importantcount = opt_mode == 0? size : 2;

    size_t half = size / 2;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distR(25.0f, 220.0f);
    std::normal_distribution<float> noise(0.0f, 6.0f);

    float bhMass = 80000.0;
    float tilt2 = g2tilt * DEG2RAD;
    float tilt1 = g1tilt * DEG2RAD;
    Vector3 axis = { 1.0f, 0.0f, 0.0f };

    Vector3 pos1 = {WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f + 100.0f, 0.0f};
    Vector3 vel1 = {45.0f, -10.0f, 0.0f};
    cstl.setBody(0, bhMass, pos1, vel1, {0}, WHITE);

    Vector3 pos2 = {WIDTH / 2.0f + 300.0f, HEIGHT / 2.0f - 100.0f, 0.0f};
    Vector3 vel2 = {-45.0f, 10.0f, 0.0f};
    cstl.setBody(1, bhMass, pos2, vel2, {0}, WHITE);   

    for (size_t i = 2; i < size; i++) {               

        bool isFirst = (i < half);
        Vector3 centerPos = isFirst ? pos1 : pos2;
        Vector3 centerVel = isFirst ? vel1 : vel2;

        float r = distR(gen);
        float angle = (i % 2) * PI + 2.0f * std::log(r) + (noise(gen) * 0.06f);

        Vector3 offPos = { r * std::cos(angle), r * std::sin(angle), noise(gen) };

        float dist = std::sqrt(offPos.x * offPos.x + offPos.y * offPos.y);
        float r2 = (float)dist * dist;
        float v = (float)std::sqrt(G * bhMass * r2 / std::pow(r2 + EPS2, 1.5));
        Vector3 offVel = { -v * (offPos.y / dist), v * (offPos.x / dist), 0.0f };

        offPos = Vector3RotateByAxisAngle(offPos, axis, isFirst ? tilt1 : tilt2);
        offVel = Vector3RotateByAxisAngle(offVel, axis, isFirst ? tilt1 : tilt2);

        Vector3 p  = Vector3Add(centerPos, offPos);
        Vector3 vv = Vector3Add(centerVel, offVel);

        cstl.setBody(i, 0.01, p, vv, {0}, WHITE);
    }
}