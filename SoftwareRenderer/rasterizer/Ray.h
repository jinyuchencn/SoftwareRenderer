#pragma once

#include "tools/Math.h"

class ray{
public:
    ray(){}
    ray(const Point3& ori){}

public:
    Point3 ori;
    Vec3 dir;

};