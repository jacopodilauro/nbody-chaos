#ifndef BODIES_H
#define BODIES_H

#include <raylib.h>
#include <vector>

struct Bodies{
    
    size_t count = 0;

    std::vector<double> mass;
    std::vector<float> pos_x, pos_y, pos_z;
    std::vector<float> vel_x, vel_y, vel_z;
    std::vector<float> acc_x, acc_y, acc_z;

    std::vector<Color> color;
    std::vector<std::vector<Vector3>> trail;

    Bodies(size_t n);

    void setBody(size_t i, double m, Vector3 p, Vector3 v, Vector3 a, Color c);
   
    void drawTrail2D(const std::vector<Vector3>& t, size_t i) const;
     
    void drawTrail3D(const std::vector<Vector3>& t, size_t i) const;
    
    void draw2D() const;

    void draw3D() const;
};

#endif
