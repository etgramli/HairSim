#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

#include  "clVectorAdd.h"

/*
typedef struct {
    cl_int3 start, end;
} vector3d;

typedef struct {
    cl_int3 coordinate;
    cl_int3 color;
    cl_uint mass;
    cl_bool isStatic;
} vertex;

typedef struct {
    cl_int3 boxStart, boxEnd;   // Two edge points that contains the area in which the force applies
    vector3d direction; // direction of the force
    cl_uint strength;
};

int areaLength = 5;     // Edge length of the area on which hairs are
int hairLength = 5;     // Number of nodes of each hair
*/

// Must be in opposite order (here AMD would be selected first, then Nvidia, at last Intel)
const std::vector<std::string> prefferedOpenClVendors = {
    "Intel(R) Corporation",
    "NVIDIA Corporation",
    "AMD"
};

// Get a platform with the desired device type, OpenCL major version, vendors in descending order
cl::Platform getPlatform(cl_device_type type,
                         unsigned int openclMajorVersion,
                         std::vector<std::string> vendors);

// Create a context for the specified platform and the stated device type
cl::Context createContext(cl_device_type type, cl::Platform *platform);


int main() {
    cl_int err = CL_SUCCESS;

    // ---- SETUP ----
    cl::Platform plat = getPlatform(CL_DEVICE_TYPE_GPU, 1, prefferedOpenClVendors);
    if (plat() == 0) {
        std::cerr << "No OpenCL platform of version 1.* found that has GPU devices!" << std::endl;
        return -1;
    } else {
        std::string info;
        err = plat.getInfo(CL_PLATFORM_NAME, &info);
        if (err == CL_SUCCESS) {
            std::cout << "Chose platform: " << std::endl
                << "\t" << plat.getInfo<CL_PLATFORM_VENDOR>() << std::endl
                << "\t" << info << std::endl
                << "\t" << plat.getInfo<CL_PLATFORM_VERSION>() << std::endl
                << "\t" << plat.getInfo<CL_PLATFORM_PROFILE>() << std::endl
                << "\t" << plat.getInfo<CL_PLATFORM_EXTENSIONS>() << std::endl << std::endl;
        }
    }
    try {
        cl::Context context = createContext(CL_DEVICE_TYPE_GPU, &plat);

        // ---- KERNEL SETUP AND LAUNCH ----
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        std::cout << "Found " << devices.size() << " devices for context" << std::endl;
        // Create cl::ConandQueue
        cl::CommandQueue queueKernel(context, devices[0], 0, &err);

        clVectorAdd *vAdd = new clVectorAdd(&context, &queueKernel);
        vAdd->enqueue();

        // Enqueue Kernel and Buffers
        std::cout << "Is result correct: " << (vAdd->validate() ? "yes" : "false") << std::endl << std::endl;

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

cl::Platform getPlatform(cl_device_type type,
                         unsigned int openclMajorVersion,
                         std::vector<std::string> vendors) {
    const std::string versionString = "OpenCL " + std::to_string(openclMajorVersion) + ".";
    cl_int err;
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);

    // Filter platforms for OpenCL version and device type
    cl::Platform plat;
    std::vector<cl::Platform> platformsWtypeANDversion;
    for (cl::Platform &current : allPlatforms) {
        std::string platformVersion = current.getInfo<CL_PLATFORM_VERSION>();
        if (platformVersion.find(versionString) != std::string::npos) {
            std::vector<cl::Device> devices;
            // ERROR CHECKING
            current.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            if (devices.size() > 0) {
                platformsWtypeANDversion.push_back(current);
            }
        }
    }
    // Select remaining platforms for vendor
    for (std::string vendor : vendors) {
        for (cl::Platform platform : platformsWtypeANDversion) {
            if (platform.getInfo<CL_PLATFORM_VENDOR>().find(vendor) != std::string::npos) {
                plat = platform;
                break;
            }
        }
    }
    return plat;
}

cl::Context createContext(cl_device_type type, cl::Platform *platform) {
    cl_context_properties *props = new cl_context_properties[3];
    props[0] = CL_CONTEXT_PLATFORM;
    props[1] = (cl_context_properties)(*platform)();
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
