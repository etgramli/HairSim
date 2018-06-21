#pragma once

#include "HairPiece.h"
#include "Link.h"

class BodySolverCPU {
private:
    HairPiece *hairPiece;
    std::vector<Vector *> forces;

    float deltaTime = 0;

    Vector addAllForces();

public:
    BodySolverCPU();
    BodySolverCPU(std::vector<Vector *> forces);
    ~BodySolverCPU();

    void pSolve_Links(const float deltaSeconds);

    float getDeltaTime() const;
    void setDeltaTime(float deltaTime);

    HairPiece* getHairPiece();
};

