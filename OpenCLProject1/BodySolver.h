#pragma once
#include "CL\cl.hpp"

#include "HairPiece.h"


class BodySolver {
private:
    std::string kernelFileName = "SolvePositionsFromLinksKernel.cl";
    std::string kernelName = "solvePositionsFromLinksKernel";

    HairPiece *hp;

    cl::Context *context;
    cl::CommandQueue *queue;

    cl::Program program;
    cl::Kernel kernel;

public:
    BodySolver(cl::Context *context, cl::CommandQueue *queue);
    ~BodySolver();

    void solveLinksForPosition();
};
