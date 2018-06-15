#include "Link.h"

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

    Vector diff = end->minus(begin);    // Difference from Node positions
    if (diff.length() > length) {           // Nodes of hair further away than length of hair -> must be pushed together
        force = diff * springConstant;
    } else if (diff.length() < length) {   // Nodes of hair closer togehter than length of hair, must be pushed away
        force = diff * -1;
    }

    return force;
};
