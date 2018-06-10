#pragma once

#include "HairPiece.h"
#include "Link.h"

class BodySolverCPU
{
private:
    HairPiece hairPiece;
    std::vector<Vector *> forces;

    Vector addAllForces() {
        Vector sum;
        for (Vector *current : forces) {
            sum += *current;
        }
    }

public:
    BodySolverCPU();
    BodySolverCPU(std::vector<Vector *> forces) {
        for (Vector *force : forces) {
            this->forces.push_back(force);
        }
    }
    ~BodySolverCPU() {
        for (Vector *force : forces) {
            delete force;
        }
    }

    void pSolve_Links();
    void pSolveSinlgeLink(Link *link);
};

