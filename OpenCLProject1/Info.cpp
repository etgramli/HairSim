#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


int printPlatformInfos() {
    int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;


    char* info;
    size_t infoSize;
    const char* attributeNames[5] = {
        "Name",
        "Vendor",
        "Version",
        "Profile",
        "Extensions" };
    const cl_platform_info attributeTypes[5] = {
        CL_PLATFORM_NAME,
        CL_PLATFORM_VENDOR,
        CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE,
        CL_PLATFORM_EXTENSIONS };
    const int attributeCount = sizeof(attributeNames) / sizeof(char*);


    // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);

    for (i = 0; i < platformCount; i++) {printf("\n\n %d. Platform \n", i+1);

    for (j = 0; j < attributeCount; j++) {
        // get platform attribute value size
        clGetPlatformInfo(platforms[i], attributeTypes[j], 0, NULL, &infoSize);
        info = (char*) malloc(infoSize);

        // get platform attribute value
        clGetPlatformInfo(platforms[i], attributeTypes[j], infoSize, info, NULL);

        printf("  %d.%d %-11s: %s\n", i+1, j+1, attributeNames[j], info);
        free(info);

    }

    // get all devices
    clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
    devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
    clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

    // for each device print critical attributes
    for (j = 0; j < deviceCount; j++) {

        // print device name
        clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
        printf("\n%d. Device: %s\n", j+1, value);
        free(value);

        clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, 0, NULL, &valueSize);
        cl_device_type *type = (cl_device_type *) malloc(valueSize);
        clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, valueSize, type, NULL);
        printf(" %d.%d Hardware version: ", j+1, 1);
        if (*type == CL_DEVICE_TYPE_CPU)
            printf("CPU");
        else if (*type == CL_DEVICE_TYPE_GPU)
            printf("GPU");
        else if (*type == CL_DEVICE_TYPE_ACCELERATOR)
            printf("Accelerator");
        else if (*type == CL_DEVICE_TYPE_DEFAULT)
            printf("Default");
        else
            printf("Unrecognized!");
        printf("\n");
        free(type);

        // print hardware device version
        clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
        printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
        free(value);

        // print software driver version
        clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
        printf(" %d.%d Software version: %s\n", j+1, 2, value);
        free(value);

        // print c version supported by compiler for device
        clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
        value = (char*) malloc(valueSize);
        clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
        printf(" %d.%d OpenCL C version: %s\n", j+1, 3, value);
        free(value);

        // print parallel compute units
        clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(maxComputeUnits), &maxComputeUnits, NULL);
        printf(" %d.%d Parallel compute units: %d\n", j+1, 4, maxComputeUnits);

    }

    free(devices);
    }

    free(platforms);
    return 0;
}
