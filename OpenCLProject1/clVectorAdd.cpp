#include "clVectorAdd.h"
#include <iostream>
#include <fstream>


clVectorAdd::clVectorAdd(cl::Context *context, cl::CommandQueue *queue)
{
    for (int i = 0; i < ARRAYSIZE; ++i) {
        A[i] = B[i] = i;
    }

    this->context = context;
    this->queue = queue;

    // ERROR CHECKING IN WHOLE FOLLOOWING BLOCK
    this->bufA = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    this->bufB = cl::Buffer(*context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    this->bufC = cl::Buffer(*context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAYSIZE);

    // Read kernel file to char array
    std::ifstream in(kernelFileName);
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    // Create cl::Program::Sources
    cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
    // Create cl::Program
    program = cl::Program(*context, src);
    // Build program
    // ERROR CHECKING
    cl_int err;
    std::vector<cl::Device> devices = (*context).getInfo<CL_CONTEXT_DEVICES>(&err);
    program.build(devices);
    // Create cl::Kernel
    this->kernel = cl::Kernel(program, this->kernelName.c_str(), &err);
}


clVectorAdd::~clVectorAdd()
{
}

cl_int clVectorAdd::enqueue() {
    std::cout << "Copying Buffers from Host to Device" << std::endl;
    cl::Event copyBufferA;
    cl::Event copyBufferB;
    std::vector<cl::Event> copyHostToDeviceEvents;
    queue->enqueueWriteBuffer(bufA, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)A, NULL, &copyBufferA);
    queue->enqueueWriteBuffer(bufB, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)B, NULL, &copyBufferB);
    copyHostToDeviceEvents.push_back(copyBufferA);  // PUSHBACK OF EVENTS MUST BE AFTER ENQUEING !!!
    copyHostToDeviceEvents.push_back(copyBufferB);
    kernel.setArg(0, bufA);
    kernel.setArg(1, bufB);
    kernel.setArg(2, bufC);
    std::cout << "Enqueue Vecotr add kernel" << std::endl;
    // Create cl::Event
    cl::Event event;
    queue->enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ARRAYSIZE), cl::NullRange, &copyHostToDeviceEvents, &event);
    // Copy back Buffer and print
    cl::Event copyBack;
    std::vector<cl::Event> copyBackWatiEvent;
    copyBackWatiEvent.push_back(event);
    queue->enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, C, &copyBackWatiEvent, &copyBack);
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
