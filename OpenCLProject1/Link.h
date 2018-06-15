#pragma once

#include "Node.h"

class Link {

    const float springConstant = 0.15f;
    const float length = 1.0f;
    const float threshold;  // 5% of length
    Node *begin, *end;

public:
    Link(Node *begin, Node *end, float length = 1.0f): begin(begin), end(end), length(length), threshold(0.05 * length) {}
    ~Link() {}
    
    // Getter
    float getSpringConstant() const {return springConstant;}
    float getLength() const {return length;}
    float getTreshold() const {return threshold;}
    Node* getBegin() const;
    Node* getEnd() const;

    Vector getSpringForce();
};
