
// TODO: Add OpenCL kernel code here.

void kernel simple_add(global const int* A,
                       global const int* B,
                       global int* C){
    unsigned int i = get_global_id(0);
    C[i]=A[i]+B[i];
}
