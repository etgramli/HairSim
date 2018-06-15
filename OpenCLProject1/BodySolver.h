#pragma once
#include "CL\cl.hpp"

#include "Link.h"


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
    std::vector<Link *> links;

    // OpenCL
    cl::Context *context;
    cl::CommandQueue *queue;

    // Buffers

    cl::Program program;
    cl::Kernel kernel;
};

