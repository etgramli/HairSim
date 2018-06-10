#include "Node.h"


Node::Node(float x, float y, float z, float a, bool isConst): x(x), y(y), z(z), isConstant(isConst)
{
}


Node::~Node()
{
}

cl_Node Node::getClData() {
    cl_float4 coordinates;
    coordinates.s0 = x;
    coordinates.s1 = y;
    coordinates.s2 = z;
    coordinates.s0 = a;

    cl_Node node;
    node.coordinates = coordinates;
    node.isConst = isConstant ? CL_TRUE : CL_FALSE;

    return node;
}
