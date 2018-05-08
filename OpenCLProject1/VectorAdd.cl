
// TODO: Add OpenCL kernel code here.

__kernel void simple_add(__global const int* A,
                       __global const int* B,
                       __global int* C){
    unsigned int i = get_global_id(0);
    C[i] = A[i] + B[i];
}
