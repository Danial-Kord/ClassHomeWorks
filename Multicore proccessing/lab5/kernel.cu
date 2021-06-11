
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <omp.h>
#include <stdio.h>

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size, int method);

__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

__global__ void addKernelWiththreadSize(int* c, const int* a, const int* b,int n)
{
    int i = threadIdx.x*n;
    int end = i + n;
    for (; i < end; i++) {
        c[i] = a[i] + b[i];
    }
}

__global__ void addKernelWithBlockSize(int* c, const int* a, const int* b)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
   c[i] = a[i] + b[i];
}

__global__ void getData(int* block, int* warp, int* thread)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    block[i] = blockIdx.x;
    thread[i] = threadIdx.x;
    warp[i] = threadIdx.x / 32;
}


const int vectorSize = 128;
void fillVector(int* v, size_t n);
void addVector(int* a, int* b, int* c, size_t n);
void printVector(int* v, size_t n);


void serial() {
    
    int a[vectorSize], b[vectorSize], c[vectorSize];

    fillVector(a, vectorSize);
    fillVector(b, vectorSize);



    //serial
    addVector(a, b, c, vectorSize);
    printVector(c, vectorSize);

}

int parallelTaskTest(int method) {

    int a[vectorSize], b[vectorSize], c[vectorSize];

    fillVector(a, vectorSize);
    fillVector(b, vectorSize);


    //   cudaEvent_t start, stop;
    //   cudaEventRecord(start);
       // Add vectors in parallel.

    //parallel
    cudaError_t cudaStatus = addWithCuda(c, a, b, vectorSize,method);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }
    printVector(c, vectorSize);
    //   cudaEventRecord(stop);
    //   float milliseconds = 0;
    //   cudaEventElapsedTime(&milliseconds, start, stop);
    //   printf("time: %f",milliseconds);




    return 0;
}

cudaError_t calcDataParallel(int* threads, int* warps, int* blocks, unsigned int size, int blockNum, int threadNum)
{
    int* dev_threads = 0;
    int* dev_blocks = 0;
    int* dev_warps = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_threads, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_blocks, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_warps, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    getData << <blockNum, threadNum >> > (dev_blocks, dev_warps, dev_threads);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(blocks, dev_blocks, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    cudaStatus = cudaMemcpy(warps, dev_warps, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    cudaStatus = cudaMemcpy(threads, dev_threads, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_warps);
    cudaFree(dev_blocks);
    cudaFree(dev_threads);

    return cudaStatus;
}

void findDataFromGPU() {
    int threads[vectorSize], blocks[vectorSize], warps[vectorSize];
    calcDataParallel(threads,warps,blocks, vectorSize, vectorSize/64,64);
    for (int i = 0; i < vectorSize; i++) {
        printf("calc thread: block: %d, warp: %d, Thread: %d\n", blocks[i], warps[i], threads[i]);
    }
}

int main(){
    double start;
    double end;
    start = omp_get_wtime();


    //serial();
    //parallelTaskTest(2);
    findDataFromGPU();
    end = omp_get_wtime();
    printf("Work took %f seconds\n", end - start);
    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size,int method=0)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    if (method == 0) {
        // Launch a kernel on the GPU with one thread for each element.
        addKernel << <1, size >> > (dev_c, dev_a, dev_b);
    }
    else if (method == 1) {
        int len = size / 1024;
        addKernelWiththreadSize << <1, 1024 >> > (dev_c, dev_a, dev_b,len);
    }
    else{
        int len = size / 1024;
        addKernelWithBlockSize << <len, 1024 >> > (dev_c, dev_a, dev_b);
    }
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}



// Fills a vector with data
void fillVector(int* v, size_t n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i;
    }
}

// Adds two vectors
void addVector(int* a, int* b, int* c, size_t n) {
#pragma loop(no_vector)
    for (int i = 0; i < n; i++) {
         c[i] = a[i] + b[i];
    }
}

// Prints a vector to the stdout.
void printVector(int* v, size_t n) {
    int i;
    printf("[-] Vector elements: ");
    for (i = 0; i < n; i++) {
        printf("%d, ", v[i]);
    }
    printf("\b\b  \n");
}