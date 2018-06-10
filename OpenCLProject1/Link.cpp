#include "Link.h"


Link::Link(Node *begin, Node *end, float restLength): begin(begin), end(end), restLength(restLength)
{
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
