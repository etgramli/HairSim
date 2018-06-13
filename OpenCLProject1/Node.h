#pragma once

#include "CL\cl.hpp"
#include "Vector.h"

// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    cl_float4 coordinates;
    cl_float mass;
    //cl_Vector *velocity;
    cl_bool isConst;
} cl_Node;


class Node {

    float x, y, z, a;
    const bool isConstant;

    Vector *velocity;
    float mass;

public:
    Node(float x, float y, float z, float a = 0, float mass = 1.0f, bool isConst = false): x(x), y(y), z(z), a(a), mass(mass), isConstant(isConst) {}
    ~Node() {}

    float getX();
    float getY();
    float getZ();
    float getA();
    float getMass();
    float getInverseMass();
    bool isConst();

    void setPosition(float x, float y, float z);

    Vector minus(Node *b);

    void move(Vector force);

    void addForce(Vector *force);

    cl_Node getClData();

    //void render();
};
