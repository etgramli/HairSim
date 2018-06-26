#pragma once
#include "CL\cl.hpp"

#include "HairPiece.h"


class BodySolver {
    std::string kernelFileName = "SolvePositionsFromLinksKernel.cl";
    std::string kernelName = "solvePositionsFromLinksKernel";
    const char *buildArgs = "-cl-mad-enable -cl-no-signed-zeros -cl-finite-math-only";

    HairPiece *hp;
    float deltaTime;
    float deltaSeconds;

    cl::Buffer bufferNodes;
    cl::Buffer bufferLinks;
    cl::Buffer bufferHairPiece;
    cl::Buffer bufferDeltaTime;
    cl::Buffer bufferDeltaSeconds;

    cl::Context *context;
    cl::CommandQueue *queue;

    cl::Program program;
    cl::Kernel kernel;


public:
    BodySolver(cl::Context *context, cl::CommandQueue *queue);
    ~BodySolver();

    void pSolve_Links(const float deltaSeconds);
    HairPiece* getHairPiece() const;
};
