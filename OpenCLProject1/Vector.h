#pragma once

#include "CL\cl.hpp"

class Vector
{
    float x,y,z;

public:
    Vector(float x, float y, float z) : x(x), y(y), z(z) {};
    ~Vector();

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }

    float getZ() {
        return z;
    }

    cl_float3 get() {
        cl_float3 value = {x,y,z};
        return value;
    }
};

