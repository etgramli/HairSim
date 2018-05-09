#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

const long ARRAYSIZE = 1024*1024;
int A[ARRAYSIZE];
int B[ARRAYSIZE];
int C[ARRAYSIZE];

// Create a context for the specified platform and the stated device type
cl::Context createContext(cl_device_type type, cl::Platform *platform);
bool validate();


int main() {
    cl_int err = CL_SUCCESS;
    // ---- SETUP ----
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform plat;
    // Select OpenCL 1.2 platform
    int counter = 0;
    for (cl::Platform &current : platforms) {
        std::string platformVersion = current.getInfo<CL_PLATFORM_VERSION>();
        if (platformVersion.find("OpenCL 1.") != std::string::npos) {
            // Check if contains device type GPU
            std::vector<cl::Device> devices;
            // ERROR CHECKING
            current.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            std::cout << "\t" << "Platform (v1.2) " << counter << " has " << devices.size() << " GPU device(s)." << std::endl;
            if (devices.size() > 0) {
                plat = current;
                std::string info;
                err = plat.getInfo(CL_PLATFORM_NAME, &info);
                if (err == CL_SUCCESS) {
                    std::cout << "Chose platform: " << info << std::endl;
                }
                err = CL_SUCCESS;
                //break;
            }
        }
        ++counter;
    }
    if (plat() == 0) {
        std::cerr << "No OpenCL platform of version 1.* found that has GPU devices!" << std::endl;
        return -1;
    } else {
        std::cout << "Found OpenCL 1.2 Platform with GPU device(s)" << std::endl;
    }
    try {
    // SetUp Context
    std::cout << "Creating context" << std::endl;
    cl::Context context = createContext(CL_DEVICE_TYPE_GPU, &plat);
    std::cout << "Context created" << std::endl;

    // ---- KERNEL SETUP AND LAUNCH ----
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    std::cout << "Found " << devices.size() << " devices for context" << std::endl;
    // Read kernel file to char array
    std::ifstream in("VectorAdd.cl");
    std::string source((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
    // Create cl::Program::Sources
    cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
    // Create cl::Program
    cl::Program program = cl::Program(context, src);
    // Build program
    // ERROR CHECKING
    program.build(devices);
    // Create cl::Kernel
    cl::Kernel kernel(program, "simple_add", &err);
    // Create cl::ConandQueue
    // If using multiple queues you must synchronize with events
    cl::CommandQueue queueIO(context, devices[0], 0, &err);     // Queue for IO (reading and writing buffers)
    cl::CommandQueue queueKernel(context, devices[0], 0, &err); // Queue for Kernels (execution)
    
    // Create cl::Event
    cl::Event event;
    // Enqueue Kernel and Buffers
    for (int i = 0; i < ARRAYSIZE; ++i) {
        A[i] = B[i] = i;
    }
    // ERROR CHECKING IN WHOLE FOLLOOWING BLOCK
    cl::Buffer bufA(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    cl::Buffer bufB(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    cl::Buffer bufC(context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAYSIZE);
    std::cout << "Copying Buffers from Host to Device" << std::endl;
    cl::Event copyBufferA;
    cl::Event copyBufferB;
    std::vector<cl::Event> copyHostToDeviceEvents;
    queueIO.enqueueWriteBuffer(bufA, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *) A, NULL, &copyBufferA);
    queueIO.enqueueWriteBuffer(bufB, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *) B, NULL, &copyBufferB);
    copyHostToDeviceEvents.push_back(copyBufferA);  // PUSHBACK OF EVENTS MUST BE AFTER ENQUEING !!!
    copyHostToDeviceEvents.push_back(copyBufferB);
    kernel.setArg(0, bufA);
    kernel.setArg(1, bufB);
    kernel.setArg(2, bufC);
    std::cout << "Enqueue Vecotr add kernel" << std::endl;
    queueKernel.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ARRAYSIZE), cl::NullRange, &copyHostToDeviceEvents, &event);
    // Copy back Buffer and print
    cl::Event copyBack;
    std::vector<cl::Event> copyBackWatiEvent;
    copyBackWatiEvent.push_back(event);
    queueIO.enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, C, &copyBackWatiEvent, &copyBack);

    std::cout << "Is result correct: " << (validate()?"yes":"false") << std::endl << std::endl;

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}


cl::Context createContext(cl_device_type type, cl::Platform *platform) {
    cl_context_properties *props = new cl_context_properties[3];
    props[0] = CL_CONTEXT_PLATFORM;
    props[1] = (cl_context_properties) (*platform)();
    props[2] = 0;

    cl_int err;
    cl::Context context;
    context = clCreateContextFromType(props, type, NULL, NULL, &err);

    std::cout << "Create context: ";
    switch (err) {
    case CL_SUCCESS:
        std::cout << "CL Success" << std::endl;
        break;
    case CL_INVALID_PROPERTY:
        std::cout << "Invalid property" << std::endl;
        break;
    case CL_INVALID_VALUE:
        std::cout << "Invalid value" << std::endl;
        break;
    case CL_INVALID_DEVICE:
        std::cout << "Invalid device" << std::endl;
        break;
    case CL_DEVICE_NOT_AVAILABLE:
        std::cout << "Device not available" << std::endl;
        break;
    case CL_OUT_OF_HOST_MEMORY:
        std::cout << "Out of host memory" << std::endl;
        break;

        default:
            std::cout << "Unknnown error on creating cl::Context!" << std::endl;
            break;
    }

    return context;
}


bool validate() {
    for (long l = 0; l < ARRAYSIZE; ++l) {
        if (C[l] != A[l] + B[l]) {
            std::cout << "Wrong value at position: " << l << " (value: " << C[l] << " = " << A[l] << "+" << B[l] << ")" << std::endl;
            return false;
        }
    }
    return true;
}
