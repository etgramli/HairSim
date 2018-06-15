#include "Link.h"
#include <iostream>
#include <math.h>

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
    
    if (diffLength > length + threshold) {           // Nodes of hair further away than length of hair -> must be pushed together
		force = diff * springConstant * abs(length - diffLength);
    } else if (diffLength < length - threshold) {   // Nodes of hair closer togehter than length of hair, must be pushed away
		force = diff * springConstant * abs(length - diffLength) * -1;
    }

	if (isnan(force.getX()) || isnan(force.getY()) || isnan(force.getZ())) {
		return Vector(0, 0, 0);
	}
	//std::cout << "SpringForce: (" << force.getX() << ", " << force.getY() << ", " << force.getZ() << ")" << std::endl;

    return force;
};
