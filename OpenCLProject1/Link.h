#pragma once

#include "Node.h"

class Link {

    //float springConstant;
    const float length = 1.0f;
    float restLength;
    Node *begin, *end;

public:
    Link(Node *begin, Node *end, float restLength = 0.1f): begin(begin), end(end), restLength(restLength) {}
    ~Link() {}
    /*
    float getSpringConstant() {
        return strength;
    }
    */

    Node* getBegin() const;
    Node* getEnd() const;
    float getRestLength() const;

    Vector getSpringForce();
};
