#pragma once

#include "Node.h"

class Link {

    //int strength;
    const float restLength;
    Node *begin, *end;

public:
    Link(Node *begin, Node *end, float restLength = 0.1f): begin(begin), end(end), restLength(restLength) {}
    ~Link() {}
    /*
    int getStrength() {
        return strength;
    }
    */

    Node* getBegin() const;
    Node* getEnd() const;
    float getRestLength() const;
};

