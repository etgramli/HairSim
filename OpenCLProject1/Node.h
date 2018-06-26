#pragma once

#include "CL\cl.hpp"
#include "Vector.h"

// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    cl_float3 coordinates;
    cl_float3 velocity;
    int isConst;
    cl_float mass;
} cl_Node;


class Node {
private:
    float x, y, z, mass;
    const bool isConstant;
	Vector velocity;

public:
    Node(const float x,
         const float y,
         const float z,
         const float mass = 1.0f,
         const bool isConst = false,
         const Vector velocity = Vector(0,0,0));
    Node(const cl_Node node);

    float getX() const;
    float getY() const;
    float getZ() const;
	float getMass() const;
    bool isConst() const;
	Vector getVelocity() const;

    void setPosition(const float x, const float y, const float z);

    Vector minus(Node const * const b);

    void move(const Vector force, const float deltaSeconds);

    cl_Node getClData() const;
};
