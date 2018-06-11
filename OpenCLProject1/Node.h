#pragma once

#include "CL\cl.hpp"
#include "Vector.h"

// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    cl_float4 coordinates;
    cl_bool isConst;
} cl_Node;


class Node {

    float x, y, z, a;
    const bool isConstant;

    Vector *velocity;
    float mass;

public:
    Node(float x, float y, float z, float a = 0, bool isConst = false): x(x), y(y), z(z), a(a), isConstant(isConst) {}
    ~Node() {}

    float getX();
    float getY();
    float getZ();
    float getA();
    bool isConst();

    void setPosition(float x, float y, float z);

    Vector minus(Node *b);

    void move(Vector force);

    void addForce(Vector *force);

    cl_Node getClData();
};
