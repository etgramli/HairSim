#include "BodySolverCPU.h"



BodySolverCPU::BodySolverCPU()
{
    //Vector *gravity = new Vector(0.0f, 0.000f, -0.0008f); // m * 9.81
    //this->forces.push_back(gravity);
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

void BodySolverCPU::pSolve_Links(const float deltaSeconds) {
    // Loop through all links, because all links must have begin and end point
    for (Link *currentLink : this->hairPiece.getLinks()) {
        if (currentLink->getBegin() != NULL && currentLink->getEnd() != NULL) {
            Node *a = currentLink->getBegin();
            Node *b = currentLink->getEnd();

            // Add gravity
			Vector gravitationalAcceleration = Vector(0.0f, 0.0f, -0.00981f);
			Vector forcesNodeA = gravitationalAcceleration * a->getMass();
			Vector forcesNodeB = gravitationalAcceleration * b->getMass();
			
			// add link force
			Link *next = hairPiece.getOutgoingLinkFor(currentLink->getEnd());
			if (next != NULL) {
				const Vector linkForce = currentLink->getLinkForce(next);
				next->getEnd()->move(linkForce * -1.0f, deltaSeconds);
			}

			//add spring force
            const Vector springForce = currentLink->getSpringForce(deltaSeconds);
			//forcesNodeA -= springForce;
			forcesNodeB += springForce;

            // Move nodes
			a->move(forcesNodeA, deltaSeconds);
			b->move(forcesNodeB, deltaSeconds);
        }
    }
}
