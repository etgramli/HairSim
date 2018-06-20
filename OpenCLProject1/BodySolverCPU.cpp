#include "BodySolverCPU.h"
#include <iostream>


BodySolverCPU::BodySolverCPU()
{
    Vector *gravity = new Vector(0.0f, 0.000f, -0.0008f);
    this->forces.push_back(gravity);
    /*
    HairPiece temp = HairPiece();
    
    cl_HairPiece cltemp = temp.getClData();
    
    hairPiece = HairPiece(cltemp);
    
    HairPiece::cleanUpClData(cltemp);

    if (hairPiece.test(temp)) {
        std::cout << "SUCCESS!";
    } else {
        std::cout << "ERROR!";
    }
    */
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
			Vector forcesNodeA = compinedForces * (0.1f * currentLink->getNum());
			Vector forcesNodeB = compinedForces * (0.1f * currentLink->getNum());

			// add link force
			Link *next = hairPiece.getOutgoingLinkFor(currentLink->getEnd());
			if (next != NULL) {
				const Vector linkForce = currentLink->getLinkForce(next);
				next->getEnd()->move(linkForce * -1.0f);
			}
            const Vector springForce = currentLink->getSpringForce();
			forcesNodeA -= springForce;
			forcesNodeB += springForce;
			
			if (a->getZ() <= 0.0f) {
				forcesNodeA = Vector(forcesNodeA.getX(), forcesNodeA.getY(), 0.0f);
			}
			if (b->getZ() <= 0.0f) {
				forcesNodeB = Vector(forcesNodeB.getX(), forcesNodeB.getY(), 0.0f);
			}
            // Move nodes
			a->move(forcesNodeA);
			b->move(forcesNodeB);
        }
    }
}
