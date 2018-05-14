#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

const long ARRAYSIZE = 1024 * 1024;
int A[ARRAYSIZE];
int B[ARRAYSIZE];
int C[ARRAYSIZE];

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
bool validate();
void printStatus(char * op, cl_int error);
char * checkError(l_int error);


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
        // Read kernel file to char array
        std::ifstream in("VectorAdd.cl");
        std::string source((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());
        // Create cl::Program::Sources
        cl::Program::Sources src(1, std::make_pair(source.c_str(), strlen(source.c_str())));
        // Create cl::Program
        cl::Program program = cl::Program(context, src);
        // Build program
        err = program.build(devices);
		printStatus("Build program:", err);
        // Create cl::Kernel
        cl::Kernel kernel(program, "simple_add", &err);
		printStatus("Create kernel:", err);
        // Create cl::ConandQueue
        // If using multiple queues you must synchronize with events
        cl::CommandQueue queueIO(context, devices[0], 0, &err);     // Queue for IO (reading and writing buffers)
		printStatus("Create IO queue:", err);
        cl::CommandQueue queueKernel(context, devices[0], 0, &err); // Queue for Kernels (execution)
		printStatus("Create kernel queue:", err);

        // Create cl::Event
        cl::Event event;
        // Enqueue Kernel and Buffers
        for (int i = 0; i < ARRAYSIZE; ++i) {
            A[i] = B[i] = i;
        }
        cl::Buffer bufA(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
		printStatus("Create buffer A:", err);
        cl::Buffer bufB(context, CL_MEM_READ_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
		printStatus("Create buffer B:", err);
        cl::Buffer bufC(context, CL_MEM_WRITE_ONLY, sizeof(int) * ARRAYSIZE, NULL, &err);
		printStatus("Create buffer C:", err);
        std::cout << "Copying Buffers from Host to Device" << std::endl;
        cl::Event copyBufferA;
        cl::Event copyBufferB;
        std::vector<cl::Event> copyHostToDeviceEvents;
        err = queueIO.enqueueWriteBuffer(bufA, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)A, NULL, &copyBufferA);
		printStatus("Copy buffer A:", err);
        err = queueIO.enqueueWriteBuffer(bufB, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, (void *)B, NULL, &copyBufferB);
		printStatus("Copy buffer B:", err);
        copyHostToDeviceEvents.push_back(copyBufferA);  // PUSHBACK OF EVENTS MUST BE AFTER ENQUEING !!!
        copyHostToDeviceEvents.push_back(copyBufferB);
        err = kernel.setArg(0, bufA);
		printStatus("Set kernel argument buffer A:", err);
        err = kernel.setArg(1, bufB);
		printStatus("Set kernel argument buffer B:", err);
        err = kernel.setArg(2, bufC);
		printStatus("Set kernel argument buffer C:", err);
        std::cout << "Enqueue Vector add kernel" << std::endl;
        err = queueKernel.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(ARRAYSIZE), cl::NullRange, &copyHostToDeviceEvents, &event);
		printStatus("Enqueue Vector add kernel:", err);
        // Copy back Buffer and print
        cl::Event copyBack;
        std::vector<cl::Event> copyBackWaitEvent;
        copyBackWatiEvent.push_back(event);
        queueIO.enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(int) * ARRAYSIZE, C, &copyBackWaitEvent, &copyBack);
		printStatus("Copy back buffer C:", err);

        std::cout << "Is result correct: " << (validate() ? "yes" : "false") << std::endl << std::endl;

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


bool validate() {
    for (long l = 0; l < ARRAYSIZE; ++l) {
        if (C[l] != A[l] + B[l]) {
            std::cout << "Wrong value at position: " << l << " (value: " << C[l] << " = " << A[l] << "+" << B[l] << ")" << std::endl;
            return false;
        }
    }
    return true;
}

void printStatus(char * op, cl_int error)
{
	if (error == 0)
	{
		printf("%s successful\n", op);
	}
	else
	{
		printf("%s failed (%s)\n", op, checkError(error));
	}
}

char * checkError(cl_int error)
{
	switch (error)
	{
	case -1:
		return "CL_DEVICE_NOT_FOUND";
	case -2:
		return "CL_DEVICE_NOT_AVAILABLE";
	case -3:
		return "CL_COMPILER_NOT_AVAILABLE";
	case -4:
		return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5:
		return "CL_OUT_OF_RESOURCES";
	case -6:
		return "CL_OUT_OF_HOST_MEMORY";
	case -7:
		return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8:
		return "CL_MEM_COPY_OVERLAP";
	case -9:
		return "CL_IMAGE_FORMAT_MISMATCH";
	case -10:
		return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11:
		return "CL_BUILD_PROGRAM_FAILURE";
	case -12:
		return "CL_MAP_FAILURE";
	case -13:
		return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14:
		return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -30:
		return "CL_INVALID_VALUE";
	case -31:
		return "CL_INVALID_DEVICE_TYPE";
	case -32:
		return "CL_INVALID_PLATFORM";
	case -33:
		return "CL_INVALID_DEVICE";
	case -34:
		return "CL_INVALID_CONTEXT";
	case -35:
		return "CL_INVALID_QUEUE_PROPERTIES";
	case -36:
		return "CL_INVALID_COMMAND_QUEUE";
	case -37:
		return "CL_INVALID_HOST_PTR";
	case -38:
		return "CL_INVALID_MEM_OBJECT";
	case -39:
		return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40:
		return "CL_INVALID_IMAGE_SIZE";
	case -41:
		return "CL_INVALID_SAMPLER";
	case -42:
		return "CL_INVALID_BINARY";
	case -43:
		return "CL_INVALID_BUILD_OPTIONS";
	case -44:
		return "CL_INVALID_PROGRAM";
	case -45:
		return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46:
		return "CL_INVALID_KERNEL_NAME";
	case -47:
		return "CL_INVALID_KERNEL_DEFINITION";
	case -48:
		return "CL_INVALID_KERNEL";
	case -49:
		return "CL_INVALID_ARG_INDEX";
	case -50:
		return "CL_INVALID_ARG_VALUE";
	case -51:
		return "CL_INVALID_ARG_SIZE";
	case -52:
		return "CL_INVALID_KERNEL_ARGS";
	case -53:
		return "CL_INVALID_WORK_DIMENSION";
	case -54:
		return "CL_INVALID_WORK_GROUP_SIZE";
	case -55:
		return "CL_INVALID_WORK_ITEM_SIZE";
	case -56:
		return "CL_INVALID_GLOBAL_OFFSET";
	case -57:
		return "CL_INVALID_EVENT_WAIT_LIST";
	case -58:
		return "CL_INVALID_EVENT";
	case -59:
		return "CL_INVALID_OPERATION";
	case -60:
		return "CL_INVALID_GL_OBJECT";
	case -61:
		return "CL_INVALID_BUFFER_SIZE";
	case -62:
		return "CL_INVALID_MIP_LEVEL";
	case -63:
		return "CL_INVALID_GLOBAL_WORK_SIZE";
	default:
		return "Unknown error";
	}
}
