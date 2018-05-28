#pragma once

#include "Node.h"


class Link {
public:
    Link(Node *begin, Node *end, int strength = 1);
    ~Link();

    int getStrength() {
        return strength;
    }

    // toClData();

    Node* getBegin();
    Node* getEnd();

private:
    int strength;
    Node *begin, *end;
};

