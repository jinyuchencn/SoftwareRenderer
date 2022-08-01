#pragma once

#include "tools/Math.h"

class Ray{
public:
    Ray(){}
    Ray(const Point3& origin,const Vec3& direction):ori(origin),dir(direction){}
    Point3 origin() const {return ori;}
    Vec3 direction() const {return dir;}

    Point3 at(double t) const {
        return ori + t * dir;
    }

public:
    Point3 ori;
    Vec3 dir;

};