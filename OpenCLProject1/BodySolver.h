#pragma once
#include "CL\cl.hpp"

#include "HairPiece.h"


class BodySolver
{
    std::string kernelFileName = "SolvePositionsFromKernel.cl";
    std::string kernelName = "SolvePositionsFromLinksKernel";

public:
    BodySolver(cl::CommandQueue *queue);
    ~BodySolver();


    void solveLinksForPosition(int startLink, 
                               int numLinks, 
                               float kst,
                               float ti);
private:
    // Data
    HairPiece hp;

    // OpenCL
    cl::Context *context;
    cl::CommandQueue *queue;

    // Buffers

    cl::Program program;
    cl::Kernel kernel;
};
