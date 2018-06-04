#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

#include "clVectorAdd.h"
#include "clHelper.h"

#include "GLwindow.h"

/*
typedef struct {
    cl_int3 start, end;
} vector3d;

typedef struct {
    cl_float4 coordinate;
    cl_uint3 color;
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
    "Advanced Micro Devices, Inc."
};

// Get a platform with the desired device type, OpenCL major version, vendors in descending order
cl::Platform getPlatform(cl_device_type type,
                         unsigned int openclMajorVersion,
                         std::vector<std::string> vendors);

// Create a context for the specified platform and the stated device type
cl::Context createContext(cl_device_type type, cl::Platform *platform);

// Open a window and start with OpenGL stuff
void openGLWindow();

int main() {
    cl_int err = CL_SUCCESS;

    // ---- SETUP ----
    cl::Platform plat = getPlatform(CL_DEVICE_TYPE_GPU, 1, prefferedOpenClVendors);
	if (plat() == 0) {
		std::cerr << "No OpenCL platform of version 1.* found that has GPU devices!" << std::endl;
		plat = getPlatform(CL_DEVICE_TYPE_GPU, 2, prefferedOpenClVendors);
		if (plat() == 0) {
			std::cerr << "No OpenCL platform of version 2.* found that has GPU devices!" << std::endl;
			return -1;
		}
	}
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

    try {
        cl::Context context = createContext(CL_DEVICE_TYPE_GPU, &plat);

        // ---- KERNEL SETUP AND LAUNCH ----
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        std::cout << "Found " << devices.size() << " devices for context" << std::endl;

        // Create cl::ConandQueue
        cl::CommandQueue queueKernel(context, devices[0], 0, &err);
        printStatus("Create kernel queue:", err);

        clVectorAdd *vAdd = new clVectorAdd(&context, &queueKernel);
        vAdd->enqueue();

        std::cout << "Is result correct: " << (vAdd->validate() ? "yes" : "false") << std::endl << std::endl;

		openGLWindow();
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

	printStatus("Create context:", err);

    return context;
}

void openGLWindow() {
	Window * window = Window::getInstance();

	window->open("Sample", 800, 600);
	window->setEyePoint(Vector4(0.0f, 0.0f, 500.0f, 1.0f));
	window->setActive();

	initializeOpenGL();

	clock_t lastInterval = clock();

	while (window->isOpen())
	{
		window->setActive();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawOpenGL(window, lastInterval);

		lastInterval = clock();

		window->swapBuffer();
	}
}
