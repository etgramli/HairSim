#pragma once

#include "HairPiece.h"
#include "Link.h"

class BodySolverCPU
{
private:
    HairPiece hairPiece;
    std::vector<Vector *> forces;

    Vector addAllForces();

public:
    BodySolverCPU();
    BodySolverCPU(std::vector<Vector *> forces);
    ~BodySolverCPU();

    void pSolve_Links();
    void pSolveSinlgeLink(Link *link);
};

