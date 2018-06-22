#pragma once

#include "Node.h"
#include <math.h>

#define PI 3.14159265f

class Link {
private:
	float springConstant;
    const float length;
    Node *begin, *end;

public:
    Link(Node *begin, Node *end, float length = 1.0f, float springConstant = 0.8f)
        : begin(begin), end(end), length(length), springConstant(springConstant) {}
    Link(const Link &link2)
        : begin(link2.getBegin()), end(link2.getEnd()), springConstant(link2.getSpringConstant()), length(link2.getLength()) {}
    
    // Getter
    float getSpringConstant() const;
    float getLength() const;
    Node* getBegin() const;
    Node* getEnd() const;

    Vector getSpringForce() const;
	Vector getLinkForce(Link const * const next) const;
};
