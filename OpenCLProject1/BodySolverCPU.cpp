#include "BodySolverCPU.h"



BodySolverCPU::BodySolverCPU()
{
    Vector *gravity = new Vector(0.0f, 0.0f, -0.5f);
    this->forces.push_back(gravity);
}

BodySolverCPU::BodySolverCPU(std::vector<Vector *> forces) {
    for (Vector *force : forces) {
        this->forces.push_back(force);
    }
}

BodySolverCPU::~BodySolverCPU() {
    for (Vector *force : forces) {
        delete force;
    }
}


float BodySolverCPU::getDeltaTime() const {
    return deltaTime;
}
void BodySolverCPU::setDeltaTime(float deltaTime) {
    this->deltaTime = deltaTime;
}

Vector BodySolverCPU::addAllForces() {
    Vector sum;
    for (Vector *current : forces) {
        sum += *current;
    }
    return sum;
}

void BodySolverCPU::pSolve_Links() {
    // Loop through all links, because all links must have begin and end point
    for (Link *currentLink : this->hairPiece.getLinks()) {
        if (currentLink->getBegin() != NULL && currentLink->getEnd() != NULL) {
            Node *a = currentLink->getBegin();
            Node *b = currentLink->getEnd();
            // Add up all forces to begin and end nodes
            const Vector compinedForces = addAllForces();
            const Vector springForce = currentLink->getSpringForce();
			const Vector forcesNodeA = compinedForces + springForce;
			const Vector forcesNodeB = compinedForces - springForce;
            // Move nodes
            a->move(forcesNodeA);
            b->move(forcesNodeB);
        }
    }
}
