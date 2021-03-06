#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

#include "clVectorAdd.h"
#include "clHelper.h"
#include "GLwindow.h"
#include "BodySolver.h"
#include "HairPiece.h"

BodySolver *bodySolver;

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
        std::cout << std::endl << "Chose platform: " << std::endl
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

        bodySolver = new BodySolver(&context, &queueKernel);
        
		openGLWindow();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    delete bodySolver;
    return 0;
}

cl::Platform getPlatform(cl_device_type type,
                         unsigned int openclMajorVersion,
                         std::vector<std::string> vendors) {
    const std::string versionString = "OpenCL " + std::to_string(openclMajorVersion) + ".";
    cl_int err = CL_SUCCESS;
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);

    // Filter platforms for OpenCL version and device type
    cl::Platform plat;
    std::vector<cl::Platform> platformsWtypeANDversion;
    for (cl::Platform &current : allPlatforms) {
        std::cout << "Plattform: " << current.getInfo<CL_PLATFORM_NAME>() << "-";
        printStatus("", err);
        std::string platformVersion = current.getInfo<CL_PLATFORM_VERSION>(&err);
        printStatus("\tGetting platform version: ", err);
        if (platformVersion.find(versionString) != std::string::npos) {
            std::vector<cl::Device> devices;
            err = current.getDevices(CL_DEVICE_TYPE_GPU, &devices);
            printStatus("\tGetting devices from platform: ", err);
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

int frameRate = 60;
float frameTime = 1.0f / ((float)frameRate);

void openGLWindow() {
	Window * window = Window::getInstance();

	window->open("Hair Simulation", 800, 600);
	window->setEyePoint(Vector4(0.0f, 100.0f, 800.0f, 1.0f));
	window->setActive();

	initializeOpenGL();

	bool render = false;
	int frames = 0;
	clock_t frameCounter = 0;
	clock_t unprocessedTime = 0;
	clock_t lastTime = clock();

	while (window->isOpen()) {
		clock_t startTime = clock();
		clock_t passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		while (unprocessedTime > frameTime) {
			render = true;

			unprocessedTime -= frameTime;

			if (frameCounter >= 1.0f) {
				float totalTime = 1000.0f * frameCounter / (float)frames;
                frames = 0;
				frameCounter = 0;
			}
		}

		if (render) {
			window->setActive();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawOpenGL(window, ((float)frameTime));
			lastTime = clock();

			window->swapBuffer();
			frames++;
		}
	}
}
