#include "BodySolver.h"

#include "clHelper.h"
#include <fstream>
#include <iostream>

BodySolver::BodySolver(cl::Context *context, cl::CommandQueue *queue) {
    this->context = context;
    this->queue = queue;

    hp = new HairPiece();


    std::cout << std::endl <<  "--- cl Kernel BodySolver setup ---" << std::endl;

    cl_int err = CL_SUCCESS;
    // ToDo: Buffers
    // Nodes
    this->bufferNodes = cl::Buffer(*context, CL_MEM_READ_WRITE, sizeof(cl_Node) * hp->getNumberOfNodes(), NULL, &err);
    printStatus("Create buffer for Nodes: ", err);
    // Links
    this->bufferLinks = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(cl_Link) * hp->getNumberOfLinks(), NULL, &err);
    printStatus("Create buffer for Links: ", err);
    // HairPiece
    this->bufferHairPiece = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(cl_HairPiece), NULL, &err);
    printStatus("Create buffer for HairPiece: ", err);
    // float deltaTime = 0.0f
    this->bufferDeltaTime = cl::Buffer(*context, CL_MEM_READ_WRITE, sizeof(cl_float), NULL, &err);
    printStatus("Create buffer for delta time: ", err);

    // Read kernel file to char array
    std::ifstream in(kernelFileName);
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
    program = cl::Program(*context, src);

    // Build program
    err = CL_SUCCESS;
    std::vector<cl::Device> devices = (*context).getInfo<CL_CONTEXT_DEVICES>(&err);
    printStatus("Context.getInfo<CL_CONTEXT_DEVICES>: ", err);
    
    err = CL_SUCCESS;
    err = program.build(devices);
    printStatus("Build program: ", err);
    if (err != CL_SUCCESS) {
        char log[65536];
        err = program.getBuildInfo(devices[0], CL_PROGRAM_BUILD_LOG, &log);
        std::cout << log << std::endl << std::endl;
    }

    // Create cl::Kernel
    this->kernel = cl::Kernel(program, this->kernelName.c_str(), &err);
    printStatus("Create kernel: ", err);

    std::cout << "--- BodySolver for OpenCL created! --- " << std::endl << std::endl;
}


BodySolver::~BodySolver() {
    delete hp;
}

void BodySolver::solveLinksForPosition(float deltaSeconds) {
    cl_HairPiece clHairPiece = hp->getClData();

    cl_int err = CL_SUCCESS;

    // Copy Array of Nodes
    cl::Event copyNodeEvent;
    err = queue->enqueueWriteBuffer(bufferNodes, CL_TRUE, 0, sizeof(cl_Node) * clHairPiece.numNodes, (void *) clHairPiece.nodes, NULL, &copyNodeEvent);
    printStatus("Enqueue copying Nodes:", err);
    // Copy Array of Links
    cl::Event copyLinkEvent;
    err = queue->enqueueWriteBuffer(bufferLinks, CL_TRUE, 0, sizeof(cl_Link) * clHairPiece.numLinks, (void *) clHairPiece.links, NULL, &copyLinkEvent);
    printStatus("Enqueue copying Nodes:", err);

    // Copy HairPiece
    cl::Event copyHairPieceEvent;
    err = queue->enqueueWriteBuffer(bufferHairPiece, CL_TRUE, 0, sizeof(clHairPiece), (void *) &clHairPiece, NULL, &copyHairPieceEvent);
    printStatus("Enqueue copying HairPiece:", err);

    // Copy deltaTime
    cl::Event copyDeltaTimeEvent;
    err = queue->enqueueWriteBuffer(bufferDeltaTime, CL_TRUE, 0, sizeof(float), (void *) &deltaTime, NULL, &copyDeltaTimeEvent);
    printStatus("Enqueue copying delta time:", err);

    // Copy deltaSeconds
    cl::Event copyDeltaSecondsEvent;
    err = queue->enqueueWriteBuffer(bufferDeltaSeconds, CL_TRUE, 0, sizeof(float), (void *) &deltaSeconds, NULL, &copyDeltaSecondsEvent);
    printStatus("Enqueue copying delta secons:", err);

    std::vector<cl::Event> copiedData;
    copiedData.push_back(copyNodeEvent);
    copiedData.push_back(copyLinkEvent);
    copiedData.push_back(copyHairPieceEvent);
    copiedData.push_back(copyDeltaTimeEvent);
    copiedData.push_back(copyDeltaSecondsEvent);

    // Set kernel arguments
    err = kernel.setArg(0, bufferHairPiece);
    printStatus("Set kernel arg 0: ", err);
    err = kernel.setArg(1, bufferNodes);
    printStatus("Set kernel arg 1: ", err);
    err = kernel.setArg(2, bufferLinks);
    printStatus("Set kernel arg 2: ", err);
    err = kernel.setArg(3, bufferDeltaTime);
    printStatus("Set kernel arg 3: ", err);
    err = kernel.setArg(4, bufferDeltaSeconds);
    printStatus("Set kernel arg 4: ", err);
    
    // Launch kernel
    cl::Event kernelEvent;
    err = queue->enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(clHairPiece.numLinks), cl::NullRange, &copiedData, &kernelEvent);
    printStatus("Launching Body Solver kernel: ", err);

    // Copy back
    std::vector<cl::Event> kernelEventVector;
    kernelEventVector.push_back(kernelEvent);
    cl::Event copyBackEvent;
    err = queue->enqueueReadBuffer(bufferNodes, CL_TRUE, 0, sizeof(cl_Node) * clHairPiece.numNodes, clHairPiece.nodes, &kernelEventVector, &copyBackEvent);
    copyBackEvent.wait();
}
