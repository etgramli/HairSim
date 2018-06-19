#pragma once

#include "CL\cl.hpp"
#include "Vector.h"

// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    cl_float4 coordinates;
    cl_float mass;
    cl_bool isConst;
} cl_Node;


class Node {

    float x, y, z, mass;
    const bool isConstant;
	Vector velocity;

public:
    Node(float x, float y, float z, float mass = 1.0f, bool isConst = false, Vector velocity = Vector(0,0,0)): x(x), y(y), z(z), mass(mass), isConstant(isConst), velocity(velocity) {}
    ~Node() {}

    float getX();
    float getY();
    float getZ();
	float getMass();
    bool isConst();
	Vector getVelocity();

    void setPosition(float x, float y, float z);

    Vector minus(Node *b);

    void move(Vector force, float deltaSeconds);

    cl_Node getClData();
};
