#include "Node.h"

cl_Node Node::getClData() {
    cl_float4 coordinates;
    coordinates.s0 = x;
    coordinates.s1 = y;
    coordinates.s2 = z;

    cl_Node node;
    node.coordinates = coordinates;
    node.isConst = isConstant ? CL_TRUE : CL_FALSE;

    return node;
}


float Node::getX() {
    return x;
}
float Node::getY() {
    return y;
}
float Node::getZ() {
    return z;
}
bool Node::isConst() {
    return isConstant;
}

void Node::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector Node::minus(Node *b) {
    if (b == NULL) {
        return Vector(x,y,z);
    } else {
        return Vector(this->x - b->getX(),
            this->y - b->getY(),
            this->z - b->getZ());
    }
}

void Node::move(Vector force) {
    if (isConstant) {
        return;
    }
    x += force.getX();
    y += force.getY();
    z += force.getZ();
}
