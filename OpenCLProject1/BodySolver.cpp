#include "BodySolver.h"

#include "clHelper.h"
#include <fstream>

BodySolver::BodySolver(cl::CommandQueue *queue)
{
    this->queue = queue;

    cl_int err;
    // ToDo: Create Program and Kernel
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


BodySolver::~BodySolver()
{
}

void BodySolver::solveLinksForPosition(int startLink,
                                       int numLinks,
                                       float kst,
                                       float ti) {
    cl_int ciErrNum = CL_SUCCESS, err = CL_SUCCESS;
    err = kernel.setArg(0, startLink);
    err |= kernel.setArg(1, numLinks);
    err |= kernel.setArg(2, kst);
    err |= kernel.setArg(3, ti);

    // Create Buffers
    // Copy Buffers
    // Set Kernel Args
    /*
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        4, 
        sizeof(cl_mem), 
        &m_linkData.m_clLinks.m_buffer);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        5,
        sizeof(cl_mem),
        &m_linkData.m_clLinksMassLSC.m_buffer);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        6, 
        sizeof(cl_mem),
        &m_linkData.m_clLinksRestLengthSquared.m_buffer);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        7, 
        sizeof(cl_mem), 
        &m_vertexData.m_clVertexInverseMass.m_buffer);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        8, 
        sizeof(cl_mem), 
        &m_vertexData.m_clVertexPosition.m_buffer);

    printStatus("Setting arguments for BodySolver kernel: ", err);

    size_t  numWorkItems = workGroupSize*
        ((numLinks + (workGroupSize-1)) / workGroupSize);
    ciErrNum = clEnqueueNDRangeKernel(
        m_cqCommandQue,
        solvePositionsFromLinksKernel,
        1,
        NULL, 
        &numWorkItems,
        &workGroupSize,0,0,0);
    if( ciErrNum!= CL_SUCCESS) {
        btAssert( 0 &&
            "enqueueNDRangeKernel(solvePositionsFromLinksKernel)");
    }*/
} // solveLinksForPosition
