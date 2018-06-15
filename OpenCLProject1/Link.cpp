#include "Link.h"
#include <iostream>
#include <math.h>

Node* Link::getBegin() const {
    return begin;
}

Node* Link::getEnd() const {
    return end;
}

float Link::getRestLength() const {
    return restLength;
}

Vector Link::getSpringForce() {
    Vector force = Vector(0,0,0); // Force from link applied to nodes

	float threshold = 0.05*length;

    Vector diff = end->minus(begin);    // Difference from Node positions
	float diffLength = diff.length();
    if (diffLength > length + threshold) {           // Nodes of hair further away than length of hair -> must be pushed together
		diff.normalize();
        force = diff * springConstant * abs(length - diffLength);
    } else if (diffLength < length - threshold) {   // Nodes of hair closer togehter than length of hair, must be pushed away
		diff.normalize();
        force = diff * springConstant * -1 * abs(length - diffLength);
    }

	if (isnan(force.getX()) || isnan(force.getY()) || isnan(force.getZ())) {
		force = Vector(0, 0, 0);
	}
	//std::cout << "SpringForce: (" << force.getX() << ", " << force.getY() << ", " << force.getZ() << ")" << std::endl;

    return force;
};
