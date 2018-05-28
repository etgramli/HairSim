#include "Link.h"


Link::Link(Node *begin, Node *end, int strength)
{
    this->begin = begin;
    this->end = end;
    this->strength = strength;
}


Link::~Link()
{
}


Node* Link::getBegin() {
    return begin;
}

Node* Link::getEnd() {
    return end;
}
