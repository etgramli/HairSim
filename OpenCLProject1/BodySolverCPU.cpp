#include "BodySolverCPU.h"
#include <iostream>


BodySolverCPU::BodySolverCPU()
{
    //Vector *gravity = new Vector(0.0f, 0.000f, -0.0008f);
    //this->forces.push_back(gravity);

    this->hairPiece = new HairPiece();
    /*
    HairPiece temp = HairPiece();
    
    cl_HairPiece cltemp = temp.getClData();
    
    hairPiece = new HairPiece(cltemp);
    
    HairPiece::cleanUpClData(cltemp);

    if (temp.test(hairPiece)) {
        std::cout << "SUCCESS!";
    } else {
        std::cout << "ERROR!";
    }*/
}

BodySolverCPU::BodySolverCPU(std::vector<Vector *> forces) {
    for (Vector *force : forces) {
        this->forces.push_back(force);
    }
}

BodySolverCPU::~BodySolverCPU() {
    delete hairPiece;
    for (Vector *force : forces) {
        delete force;
    }
    forces.clear();
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
    for (Link *currentLink : this->hairPiece->getLinks()) {
        if (currentLink->getBegin() != NULL && currentLink->getEnd() != NULL) {
            Node *a = currentLink->getBegin();
            Node *b = currentLink->getEnd();
            
            // add gravity
			Vector gravitationalAcceleration = Vector(0.0f, 0.0f, -0.00981f);
			Vector forcesNodeA = gravitationalAcceleration * a->getMass();
			Vector forcesNodeB = gravitationalAcceleration * b->getMass();
			
			// add link force
			Link *pre = hairPiece->getIngoingLinkFor(currentLink->getBegin());
			if (pre != NULL) {
				const Vector linkForce = currentLink->getLinkForce(pre);
				forcesNodeB -= linkForce;
			}

			//add spring force
            const Vector springForce = currentLink->getSpringForce(deltaSeconds);
			forcesNodeB += springForce;

			// add wind
			deltaTime += deltaSeconds;
			const Vector windForce = Vector(0.09f, -0.08f, 0.05f) * (sin(deltaTime * 0.02f) + 1.0f);
			forcesNodeA += windForce;
			forcesNodeB += windForce;

            // Move nodes
			a->move(forcesNodeA, deltaSeconds);
			b->move(forcesNodeB, deltaSeconds);
        }
    }
}
