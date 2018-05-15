#include "clVectorAdd.h"
#include "clHelper.h"
#include <iostream>
#include <fstream>


clVectorAdd::clVectorAdd(cl::Context *context, cl::CommandQueue *queue)
{
    cl_int err;
    for (int i = 0; i < ARRAYSIZE; ++i) {
        A[i] = B[i] = i;
    }

    this->context = context;
    this->queue = queue;

    // ERROR CHECKING IN WHOLE FOLLOOWING BLOCK
    this->bufA = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
    printStatus("Create buffer A:", err);
    this->bufB = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
    printStatus("Create buffer B:", err);
    this->bufC = cl::Buffer(*context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
    printStatus("Create buffer C:", err);

    // Read kernel file to char array
    std::ifstream in(kernelFileName);
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    // Create cl::Program::Sources
    cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
    // Create cl::Program
    program = cl::Program(*context, src);
    // Build program
    std::vector<cl::Device> devices = (*context).getInfo<CL_CONTEXT_DEVICES>(&err);
    printStatus("Context.getInfo<CL_CONTEXT_DEVICES>:", err);

    err = CL_SUCCESS;
    err = program.build(devices);
    printStatus("Build program:", err);

    // Create cl::Kernel
    this->kernel = cl::Kernel(program, this->kernelName.c_str(), &err);
    printStatus("Create kernel:", err);
}


clVectorAdd::~clVectorAdd()
{
}

cl_int clVectorAdd::enqueue() {
    cl_int err = CL_SUCCESS;
    std::cout << "Copying Buffers from Host to Device" << std::endl;
    cl::Event copyBufferA;
    cl::Event copyBufferB;
    std::vector<cl::Event> copyHostToDeviceEvents;
    err = queue->enqueueWriteBuffer(bufA, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)A, NULL, &copyBufferA);
    printStatus("Copy buffer A:", err);
    err = queue->enqueueWriteBuffer(bufB, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)B, NULL, &copyBufferB);
    printStatus("Copy buffer B:", err);
    copyHostToDeviceEvents.push_back(copyBufferA);  // PUSHBACK OF EVENTS MUST BE AFTER ENQUEING !!!
    copyHostToDeviceEvents.push_back(copyBufferB);
    err = kernel.setArg(0, bufA);
    printStatus("Set kernel arg 0:", err);
    err = kernel.setArg(1, bufB);
    printStatus("Set kernel arg 1:", err);
    err = kernel.setArg(2, bufC);
    printStatus("Set kernel arg 2:", err);
    std::cout << "Enqueue Vecotr add kernel" << std::endl;
    // Create cl::Event
    cl::Event event;
    err = queue->enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ARRAYSIZE), cl::NullRange, &copyHostToDeviceEvents, &event);
    printStatus("Enqueue kernel Vector add:", err);

    // Copy back Buffer and print
    cl::Event copyBack;
    std::vector<cl::Event> copyBackWatiEvent;
    copyBackWatiEvent.push_back(event);
    err = queue->enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, C, &copyBackWatiEvent, &copyBack);
    printStatus("Copy back buffer C:", err);
    copyBack.wait();

    return 0;
}

bool clVectorAdd::validate() {
    for (long l = 0; l < ARRAYSIZE; ++l) {
        if (C[l] != A[l] + B[l]) {
            std::cout << "Wrong value at position: " << l << " (value: " << C[l] << " = " << A[l] << "+" << B[l] << ")" << std::endl;
            return false;
        }
    }
    return true;
}
