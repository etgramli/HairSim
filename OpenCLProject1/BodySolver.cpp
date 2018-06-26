#include "BodySolver.h"

#include "clHelper.h"
#include <fstream>
#include <iostream>

BodySolver::BodySolver(cl::Context *context, cl::CommandQueue *queue) {
    this->deltaTime = 0;
    this->deltaSeconds = 0;
    hp = new HairPiece();

    this->context = context;
    this->queue = queue;
    std::cout << std::endl <<  "--- cl Kernel BodySolver setup ---" << std::endl;

    cl_int err = CL_SUCCESS;
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
    // float deltaSeconds
    this->bufferDeltaSeconds = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(cl_float), NULL, &err);
    printStatus("Create buffer for delta seconds: ", err);

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
    err = program.build(devices, buildArgs);
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

void BodySolver::pSolve_Links(float deltaSeconds) {
    this->deltaSeconds = deltaSeconds;
    cl_HairPiece clHairPiece = hp->getClData();

    cl_int err = CL_SUCCESS;

    // Copy Array of Nodes
    cl::Event copyNodeEvent;
    err = queue->enqueueWriteBuffer(bufferNodes, CL_TRUE, 0, sizeof(cl_Node) * clHairPiece.numNodes, (void *) clHairPiece.nodes, NULL, &copyNodeEvent);
    if (err != CL_SUCCESS)printStatus("Enqueue copying Nodes:", err);
    // Copy Array of Links
    cl::Event copyLinkEvent;
    err = queue->enqueueWriteBuffer(bufferLinks, CL_TRUE, 0, sizeof(cl_Link) * clHairPiece.numLinks, (void *) clHairPiece.links, NULL, &copyLinkEvent);
    if (err != CL_SUCCESS)printStatus("Enqueue copying Nodes:", err);

    // Copy HairPiece
    cl::Event copyHairPieceEvent;
    err = queue->enqueueWriteBuffer(bufferHairPiece, CL_TRUE, 0, sizeof(clHairPiece), (void *) &clHairPiece, NULL, &copyHairPieceEvent);
    if (err != CL_SUCCESS)printStatus("Enqueue copying HairPiece:", err);

    // Copy deltaTime
    cl::Event copyDeltaTimeEvent;
    err = queue->enqueueWriteBuffer(bufferDeltaTime, CL_TRUE, 0, sizeof(float), (void *) &deltaTime, NULL, &copyDeltaTimeEvent);
    if (err != CL_SUCCESS)printStatus("Enqueue copying delta time:", err);

    // Copy deltaSeconds
    cl::Event copyDeltaSecondsEvent;
    err = queue->enqueueWriteBuffer(bufferDeltaSeconds, CL_TRUE, 0, sizeof(float), (void *) &this->deltaSeconds, NULL, &copyDeltaSecondsEvent);
    if (err != CL_SUCCESS)printStatus("Enqueue copying delta secons:", err);

    std::vector<cl::Event> copiedData;
    copiedData.push_back(copyNodeEvent);
    copiedData.push_back(copyLinkEvent);
    copiedData.push_back(copyHairPieceEvent);
    copiedData.push_back(copyDeltaTimeEvent);
    copiedData.push_back(copyDeltaSecondsEvent);

    // Set kernel arguments
    err = kernel.setArg(0, bufferHairPiece);
    if (err != CL_SUCCESS)printStatus("Set kernel arg 0: ", err);
    err = kernel.setArg(1, bufferNodes);
    if (err != CL_SUCCESS)printStatus("Set kernel arg 1: ", err);
    err = kernel.setArg(2, bufferLinks);
    if (err != CL_SUCCESS)printStatus("Set kernel arg 2: ", err);
    err = kernel.setArg(3, bufferDeltaTime);
    if (err != CL_SUCCESS)printStatus("Set kernel arg 3: ", err);
    err = kernel.setArg(4, bufferDeltaSeconds);
    if (err != CL_SUCCESS)printStatus("Set kernel arg 4: ", err);
    
    // Launch kernel
    cl::Event kernelEvent;
    err = queue->enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(clHairPiece.numLinks), cl::NullRange, &copiedData, &kernelEvent);
    if (err != CL_SUCCESS)printStatus("Launching Body Solver kernel: ", err);

    std::vector<cl::Event> kernelEventVector;
    kernelEventVector.push_back(kernelEvent);

    // Copy back
    cl::Event copyNodesBackEvent;
    err = queue->enqueueReadBuffer(bufferNodes, CL_TRUE, 0, sizeof(cl_Node) * clHairPiece.numNodes, clHairPiece.nodes, &kernelEventVector, &copyNodesBackEvent);
    if (err != CL_SUCCESS)printStatus("Copy back nodes: ", err);

    cl::Event copyLinksBackEvent;
    err = queue->enqueueReadBuffer(bufferLinks, CL_TRUE, 0, sizeof(cl_Link) * clHairPiece.numLinks, clHairPiece.links, &kernelEventVector, &copyLinksBackEvent);
    if (err != CL_SUCCESS)printStatus("Copy back links: ", err);

    cl::Event copyDeltaTimeBackEvent;
    err = queue->enqueueReadBuffer(bufferDeltaTime, CL_TRUE, 0, sizeof(cl_float), &deltaTime, &kernelEventVector, &copyDeltaTimeBackEvent);
    if (err != CL_SUCCESS)printStatus("Copy back delta links: ", err);

    copyNodesBackEvent.wait();
    copyLinksBackEvent.wait();
    copyDeltaTimeBackEvent.wait();

    // Convert back to CPU data types
    delete hp;
    hp = new HairPiece(clHairPiece);
}

HairPiece* BodySolver::getHairPiece() const {
    return hp;
}
