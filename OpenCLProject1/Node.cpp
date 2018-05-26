#include "Node.h"


Node::Node(float x, float y, float z, float a, bool isConst)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->a = a;
    this->isConstant = isConst;
}


Node::~Node()
{
}

cl_float4 Node::getClData() {
    cl_float4 node;
    node.s0 = x;
    node.s1 = y;
    node.s2 = z;
    node.s0 = a;
    return node;
}
