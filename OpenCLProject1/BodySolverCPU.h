#pragma once

#include "HairPiece.h"
#include "Link.h"

class BodySolverCPU {

    // Constants
    /*
    const float DEFAULT_DAMPING =  -0.0125f;
    const float KsStruct = 50.75f;
    const float KdStruct = -0.25f;
    const float KsShear = 50.75f;
    const float KdShear = -0.25f;
    const float KsBend = 50.95f;
    const float KdBend = -0.25f;
    */


    HairPiece hairPiece;
    std::vector<Vector *> forces;

    float deltaTime = 1;

    Vector addAllForces();

public:
    BodySolverCPU();
    BodySolverCPU(std::vector<Vector *> forces);
    ~BodySolverCPU();

    void pSolve_Links();
    void pSolveSinlgeLink(Link *link);

    float getDeltaTime() const;
    void setDeltaTime(float deltaTime);
};

