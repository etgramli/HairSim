#include "Link.h"


float Link::getSpringConstant() const {
    return springConstant;
}

float Link::getLength() const {
    return length;
}

Node* Link::getBegin() const {
    return begin;
}

Node* Link::getEnd() const {
    return end;
}

Vector Link::getSpringForce() {
    Vector diff = end->minus(begin);    // Difference from Node positions
	float diffLength = diff.length();
	diff.normalize();
	float s = length - diffLength;

    return diff * (springConstant * s);;
};

Vector Link::getLinkForce(Link *pre) {
	Vector a = pre->begin->minus(pre->end);
	Vector b = end->minus(begin);
	Vector diff = a + b;
	float diffLength = diff.length();
	diff.normalize();
	float angle = acos((a * b) / (a.length() * b.length())) * 180.0f / PI;
	if (angle < 178) {
		return diff * (0.3f * diffLength);
    } else {
        return Vector(0, 0, 0);
    }
};
