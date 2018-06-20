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

Vector Link::getLinkForce(Link * pre) {
	Vector force = Vector(0, 0, 0);
	
	Vector a = pre->begin->minus(pre->end);
	Vector b = end->minus(begin);
	Vector diff = a + b;
	float diffLength = diff.length();
	diff.normalize();
	float angle = acos((a * b) / (a.length() * b.length())) * 180.0f / PI;
	if (angle < 178) {
		force = diff * (0.3f * diffLength);
	}

	return force;
};
