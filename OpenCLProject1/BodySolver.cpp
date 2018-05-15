#include "BodySolver.h"


BodySolver::BodySolver(cl::CommandQueue *queue)
{
    this->queue = queue;

    // ToDo: Create Program and Kernel
}


BodySolver::~BodySolver()
{
}

void BodySolver::solveLinksForPosition(int startLink,
                                       int numLinks,
                                       float kst,
                                       float ti) {
    /*
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        0, 
        sizeof(int), 
        &startLink);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        1, 
        sizeof(int), 
        &numLinks);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        2, 
        sizeof(float), 
        &kst);
    ciErrNum = clSetKernelArg(
        solvePositionsFromLinksKernel,
        3, 
        sizeof(float), 
        &ti);
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
    }
    */
} // solveLinksForPosition
