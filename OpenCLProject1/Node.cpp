#include "Node.h"

cl_Node Node::getClData() {
    cl_Node node;
    node.coordinates = cl_float3();
    node.coordinates.x = x;
    node.coordinates.y = y;
    node.coordinates.z = z;
    node.isConst = isConstant ? CL_TRUE : CL_FALSE;
	node.mass = mass;
	node.velocity = cl_float3();
	node.velocity.x = velocity.getX();
	node.velocity.y = velocity.getY();
	node.velocity.z = velocity.getZ();

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
float Node::getMass() {
	return mass;
}
bool Node::isConst() {
    return isConstant;
}
Vector Node::getVelocity() {
	return velocity;
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

void Node::move(Vector force, float deltaSeconds) {
    if (isConstant) {
        return;
    }
	Vector v = force * (deltaSeconds / mass) + (velocity * 0.9f);// damping factor
	x += v.getX();
	y += v.getY();
	z += v.getZ();
	velocity = v;

	if (z <= 0) {
		z = 0;
		if (velocity.getZ() <= 0) {
			velocity = Vector(velocity.getX(), velocity.getY(), 0);
		}
	}
}
