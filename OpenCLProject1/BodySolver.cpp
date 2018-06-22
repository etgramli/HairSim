#include "BodySolver.h"

#include "clHelper.h"
#include <fstream>
#include <iostream>

BodySolver::BodySolver(cl::Context *context, cl::CommandQueue *queue) {
    this->context = context;
    this->queue = queue;

    hp = new HairPiece();


    // ToDo: Buffers
    // Nodes
    // Links
    // HairPiece
    // float deltaTime = 0.0f


    std::cout << std::endl <<  "--- cl Kernel BodySolver setup ---" << std::endl;

    // Read kernel file to char array
    std::ifstream in(kernelFileName);
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
    program = cl::Program(*context, src);

    // Build program
    cl_int err = CL_SUCCESS;
    std::vector<cl::Device> devices = (*context).getInfo<CL_CONTEXT_DEVICES>(&err);
    printStatus("Context.getInfo<CL_CONTEXT_DEVICES>:", err);
    
    err = CL_SUCCESS;
    err = program.build(devices);
    printStatus("Build program:", err);
    if (err != CL_SUCCESS) {
        char log[65536];
        err = program.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &log);
        std::cout << log << std::endl << std::endl;
    }

    // Create cl::Kernel
    this->kernel = cl::Kernel(program, this->kernelName.c_str(), &err);
    printStatus("Create kernel:", err);

    std::cout << "--- BodySolver for OpenCL created! --- " << std::endl << std::endl;
}


BodySolver::~BodySolver() {
    delete hp;
}

void BodySolver::solveLinksForPosition() {
    cl_int err = CL_SUCCESS;

    // Copy Array of Nodes
    // Copy Array of Links
    // Copy HairPiece
    // - Set Pointer in HairPiece to Array of Nodes
    // - Set Pointer in HairPiece to Array of Links

    /*
    err = kernel.setArg(0, startLink);
    err |= kernel.setArg(1, numLinks);
    err |= kernel.setArg(2, kst);
    err |= kernel.setArg(3, ti);
    */
}
