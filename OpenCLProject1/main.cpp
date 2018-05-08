#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

const int ARRAYSIZE = 256;
int A[ARRAYSIZE] = {1};
int B[ARRAYSIZE] = {5};
int C[ARRAYSIZE];

cl::Context createContext(cl_device_type type, cl::Platform *platform);


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
            current.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            std::cout << "\t" << "Platform (v1.2) " << counter << " has " << devices.size() << " GPU device(s)." << std::endl;
            if (devices.size() > 0) {
                plat = current;
                break;
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
    program.build(devices);
    // Create cl::Kernel
    cl::Kernel kernel(program, "simple_add", &err);
    // Create cl::ConandQueue
    cl::CommandQueue queue(context, devices[0], 0, &err);
    
    // Create cl::Event
    cl::Event event;
    // Enqueue Kernel and Buffers
    for (int i = 0; i < ARRAYSIZE; ++i) {
        A[i] = B[i] = i;
    }
    cl::Buffer bufA(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    cl::Buffer bufB(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE);
    cl::Buffer bufC(context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAYSIZE);
    std::cout << "Copying Buffers from Host to Device" << std::endl;
    cl::Event copyToDevice;
    queue.enqueueWriteBuffer(bufA, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *) A);
    queue.enqueueWriteBuffer(bufB, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *) B, NULL, &copyToDevice);
    kernel.setArg(0, bufA);
    kernel.setArg(1, bufB);
    kernel.setArg(2, bufC);
    std::cout << "Enqueue Vecotr add kernel" << std::endl;
    copyToDevice.wait();
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ARRAYSIZE), cl::NullRange, NULL, &event);
    
    // Wait
    event.wait();
    std::cout << "Vecotr add kernel completed" << std::endl;

    // Copy back Buffer and print
    cl::Event copyBack;
    queue.enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, C, NULL, &copyBack);
    copyBack.wait();

    for (int i = 0; i < ARRAYSIZE; ++i) {
        std::cout << C[i] << ", ";
    }
    std::cout << std::endl << std::endl;

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
    //context = cl::Context(type, props, NULL, NULL, &err);

    // ToDo: Analyze err
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
