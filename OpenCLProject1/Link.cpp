#include "Link.h"


Node* Link::getBegin() const {
    return begin;
}

Node* Link::getEnd() const {
    return end;
}

Vector Link::getSpringForce(float t) {
    Vector force = Vector(0,0,0); // Force from link applied to nodes

    Vector diff = end->minus(begin);    // Difference from Node positions
	float diffLength = diff.length();
	diff.normalize();
	float s = length - diffLength;

	force = diff * (springConstant * s);


	
    return force;
};

Vector Link::getLinkForce(Link * next) {
	Vector force = Vector(0, 0, 0);
	
	Vector a = begin->minus(end);
	Vector b = next->end->minus(next->begin);
	float angle = acos((a * b) / (a.length() * b.length())) * 180.0f / PI;
	if (angle < 180 - next->getNum() * 4) {
		force = (a + b) * 0.05f;
	}

	return force;
};
