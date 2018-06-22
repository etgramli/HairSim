#pragma once
#include "CL\cl.hpp"

class clVectorAdd {
    std::string kernelFileName = "VectorAdd.cl";
    std::string kernelName = "simple_add";
    static const long ARRAYSIZE = 1024 * 1024;
    int A[ARRAYSIZE];
    int B[ARRAYSIZE];
    int C[ARRAYSIZE];

    cl::Context *context;
    cl::CommandQueue *queue;

    cl::Buffer bufA;
    cl::Buffer bufB;
    cl::Buffer bufC;

    cl::Program program;
    cl::Kernel kernel;


public:
    clVectorAdd(cl::Context *context, cl::CommandQueue *queue);

    cl_int enqueue();
    bool validate();
};
