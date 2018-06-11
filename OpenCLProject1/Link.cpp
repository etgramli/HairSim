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
