#pragma once

#include "CL\cl.hpp"
#include "Vector.h"

// Type definition must be the same as in kernel,
// so that it can be copied from host to device
typedef struct {
    cl_float3 coordinates;
    cl_bool isConst;
} cl_Node;


class Node {

    float x, y, z;
    const bool isConstant;

public:
    Node(float x, float y, float z, bool isConst = false): x(x), y(y), z(z), isConstant(isConst) {}
    Node(const cl_Node &node)
        : x(node.coordinates.x), y(node.coordinates.y), z(node.coordinates.z), isConstant(node.isConst) {}
    ~Node() {}

    float getX();
    float getY();
    float getZ();
    bool isConst();

    void setPosition(float x, float y, float z);

    Vector minus(Node *b);

    void move(Vector force);

    cl_Node getClData();
};
