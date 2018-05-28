#pragma once

#include "CL\cl.hpp"


class Node {
public:
    Node(float x, float y, float z, float a = 0, bool isConst = false);
    ~Node();

    float getX() {
        return x;
    }
    float getY() {
        return y;
    }
    float getZ() {
        return z;
    }
    float getA() {
        return a;
    }
    bool isConst() {
        return isConstant;
    }

    cl_float4 getClData();

private:
    float x, y, z, a;
    bool isConstant;
};
