#ifndef BODY_H
#define BODY_H

#include <raylib.h>

class Body{
    
    public: 
        double mass;
        Vector3 pos;
        Vector3 vel;
        Vector3 acc;
        Color color;
        std::vector<Vector3> trajectory; // soon

    public: 

        Body() : mass (1), pos ({0}), 
            vel ({0}), acc ({0}), color (WHITE) {}

        Body(double m, Vector3 p, Vector3 v, Vector3 a, Color c) : 
            mass (m), pos (p), vel (v), acc (a), color (c) {}

        void draw2D(){
            DrawCircle(pos.x, pos.y, 5, color);
        }

        void draw3D(){
            DrawSphere(pos, 20, color); 
        }

        void setValues(double m, Vector3 p, Vector3 v, Vector3 a, Color c) {
            mass = m;
            pos = p;
            vel = v;
            acc = a;
            color = c;
        }

        void setMass(double m) {mass = m;}
        void setPos(Vector3 p) {pos = p;}
        void setVel(Vector3 v) {vel = v;}
        void setAcc(Vector3 a) {acc = a;}
        void setColor(Color c) {color = c;}

        double getMass(){return mass;}
        Vector3 getPos(){return pos;}
        Vector3 getAcc(){return acc;}
        Color getColor(){return color;}

};

#endif
