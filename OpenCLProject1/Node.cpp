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


float Node::getX() const {
    return x;
}
float Node::getY() const {
    return y;
}
float Node::getZ() const {
    return z;
}
float Node::getMass() const {
	return mass;
}
bool Node::isConst() const {
    return isConstant;
}
Vector Node::getVelocity() const {
	return velocity;
}

void Node::setPosition(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector Node::minus(Node const * const b) {
    if (b == NULL) {
        return Vector(x,y,z);
    } else {
        return Vector(this->x - b->getX(),
            this->y - b->getY(),
            this->z - b->getZ());
    }
}

void Node::move(const Vector force, const float deltaSeconds) {
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
