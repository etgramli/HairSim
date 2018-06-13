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
    Vector diff = begin->minus(end);
    if (diff.length() > length) {
        diff.subtract(restLength);  // springDisplacement
    } else if (diff.length() < length){
        diff.add(restLength);
    }

    // ToDo

    return diff;
};
