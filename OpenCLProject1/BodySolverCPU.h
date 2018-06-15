#pragma once

#include "HairPiece.h"
#include "Link.h"

class BodySolverCPU {

    HairPiece hairPiece;
    std::vector<Vector *> forces;

    float deltaTime = 1;

    Vector addAllForces();

public:
    BodySolverCPU();
    BodySolverCPU(std::vector<Vector *> forces);
    ~BodySolverCPU();

    void pSolve_Links();

    float getDeltaTime() const;
    void setDeltaTime(float deltaTime);

    HairPiece* getHairPiece() {
        return &hairPiece;
    }
};

