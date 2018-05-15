#pragma once
#include "CL\cl.hpp"


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
    cl::Context *context;
    cl::CommandQueue *queue;

    // Buffers

    cl::Program program;
    cl::Kernel kernel;
};

