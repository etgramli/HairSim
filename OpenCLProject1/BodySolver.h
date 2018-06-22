#pragma once
#include "CL\cl.hpp"

#include "HairPiece.h"


class BodySolver {
    std::string kernelFileName = "SolvePositionsFromLinksKernel.cl";
    std::string kernelName = "solvePositionsFromLinksKernel";

    HairPiece *hp;
    float deltaTime = 0;

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

    void solveLinksForPosition(const float deltaSeconds);
};
