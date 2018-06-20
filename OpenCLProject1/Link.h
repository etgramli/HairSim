#pragma once

#include "Node.h"
#include <math.h>

#define PI 3.14159265f

class Link {
	float springConstant;
    const float length;
	int num;
    const float threshold;  // 5% of length
    Node *begin, *end;

public:
    Link(Node *begin, Node *end, int num, float length = 1.0f, float springConstant = 0.8f)
        : begin(begin), end(end), length(length), num(num), springConstant(springConstant), threshold(0.05 * length) {}
    Link(const Link &link2)
        : begin(link2.getBegin()), end(link2.getEnd()), springConstant(link2.getSpringConstant()),
        length(link2.getLength()), num(link2.getNum()), threshold (link2.getTreshold()) {}
    ~Link() {}
    
    // Getter
    float getSpringConstant() const {return springConstant;}
    float getLength() const {return length;}
    float getTreshold() const {return threshold;}
	int getNum() const { return num; }
    Node* getBegin() const;
    Node* getEnd() const;

    Vector getSpringForce(float t);
	Vector getLinkForce(Link * next);
};
