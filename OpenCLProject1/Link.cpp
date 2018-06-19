#include "Link.h"


Node* Link::getBegin() const {
    return begin;
}

Node* Link::getEnd() const {
    return end;
}

Vector Link::getSpringForce() {
    Vector force = Vector(0,0,0); // Force from link applied to nodes

    Vector diff = end->minus(begin);    // Difference from Node positions
	float diffLength = diff.length();
	diff.normalize();
    if (diffLength > length + threshold) {           // Nodes of hair further away than length of hair -> must be pushed together
		force = diff * springConstant * (length - diffLength);// * abs(length - diffLength);
    } else if (diffLength < length - threshold) {   // Nodes of hair closer togehter than length of hair, must be pushed away
		force = diff * springConstant * (length - diffLength);
	}
	else {
		//force = Vector(0.0f, 0.0f, 0.005f);
	}
    return force;
};

Vector Link::getLinkForce(Link * next) {
	Vector force = Vector(0, 0, 0);
	Vector a = begin->minus(end);
	Vector b = next->end->minus(next->begin);
	float angle = acos((a * b) / (a.length() * b.length())) * 180.0f / PI;
	if (angle < 178 - next->getNum() * 4) {
		force = (a + b) * (0.001f * next->getNum());
	}
	return force;
};
