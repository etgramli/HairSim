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

    void setPosition(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector minus(Node *b) {
        if (b == NULL) {
            return Vector(x,y,z);
        } else {
            return Vector(this->x - b->getX(),
                this->y - b->getY(),
                this->z - b->getZ());
        }
    }

    void move(Vector force) {
        if (isConstant) {
            return;
        }
        x += force.getX();
        y += force.getY();
        z = force.getZ();
    }

    void addForce(Vector *force) {
        *this->velocity += *force;
    }

    cl_Node getClData();

private:
    float x, y, z, a;
    const bool isConstant;

    Vector *velocity;
    float mass;
};
