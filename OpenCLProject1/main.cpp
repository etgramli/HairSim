#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CL/cl.hpp>


int main() {
    // ---- SETUP ----
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform plat;
    // Select OpenCL 1.2 platform
    for (cl::Platform &current : platforms) {
        std::string platformVersion = current.getInfo<CL_PLATFORM_VERSION>();
        if (platformVersion.find("OpenCL 1.") != std::string::npos) {
            // Check if contains device type GPU
            std::vector<cl::Device> devices;
            current.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            if (devices.size() > 0) {
                plat = current;
            }
        }
    }
    if (plat() == 0) {
        std::cerr << "No OpenCL platform of version 1.* found that has GPU devices!" << std::endl;
        return -1;
    }
    // SetUp Context
    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties) &plat, 0
    };
    cl::Context context(CL_DEVICE_TYPE_GPU, properties);


    // ---- KERNEL SETUP AND LAUNCH ----
    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>;
    // ToDo: Read kernel file to char array
    // Create cl::Program::Sources
    // Create cl::Program
    // Build program
    // Create cl::Kernel
    // Create cl::ConandQueue
    // Create cl::Event
    // Enqueue Event
    // Wait

    return 0;
}
