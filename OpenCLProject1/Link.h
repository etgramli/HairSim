#pragma once

#include "Node.h"

class Link {
public:
    Link(Node *begin, Node *end, float restLength = 0.1);
    ~Link();
    /*
    int getStrength() {
        return strength;
    }
    */

    float getRestLength() const {
        return restLength;
    }

    Node* getBegin();
    Node* getEnd();

private:
    //int strength;
    const float restLength;
    Node *begin, *end;
};

