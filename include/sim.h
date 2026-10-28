#ifndef SIM_H
#define SIM_H

#if defined(USE_OMP)
  #define OMP_PARALLEL_FOR      _Pragma("omp parallel for schedule(static)")
  #define OMP_PARALLEL_FOR_SIMD _Pragma("omp parallel for simd schedule(static)")
#else
  #define OMP_PARALLEL_FOR
  #define OMP_PARALLEL_FOR_SIMD _Pragma("omp simd")
#endif

#include <raylib.h>

#include "body.h"

void InitNBodies(Bodies &cstl, float WIDTH, float HEIGHT, int seed, float pa, float pb, float va, float vb);

void UpdateState(Bodies &cstl, float dt);

void UpdateGalaxyColor(Bodies &cstl);

void UpdateTrail(Bodies &cstl);

void DrawCustomGrid(Vector3 center, int slices, float spacing) ;

void InitGalaxyCollision(Bodies &b, float WIDTH, float HEIGHT, int seed, unsigned int opt_mode, float g1tilt, float g2tilt) ;

#endif
