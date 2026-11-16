#include "CudaTest.cuh"

#include <cuda_runtime.h>
#include <iostream>

__global__ void IncrementKernel(int* value)
{
    *value += 1;
}

bool RunCudaTest()
{
    int hostValue = 41;
    int result = 0;

    int* deviceValue = nullptr;

    // Allocate memory on the GPU.
    cudaError_t error = cudaMalloc(
        reinterpret_cast<void**>(&deviceValue),
        sizeof(int)
    );

    if (error != cudaSuccess)
    {
        std::cerr << "cudaMalloc failed: "
            << cudaGetErrorString(error) << '\n';
        return false;
    }

    bool success = false;

    do
    {
        // Copy the value from CPU memory to GPU memory.
        error = cudaMemcpy(
            deviceValue,
            &hostValue,
            sizeof(int),
            cudaMemcpyHostToDevice
        );

        if (error != cudaSuccess)
        {
            std::cerr << "Host-to-device copy failed: "
                << cudaGetErrorString(error) << '\n';
            break;
        }

        // Launch one CUDA thread in one block.
        IncrementKernel << <1, 1 >> > (deviceValue);

        // Check whether the kernel launch itself failed.
        error = cudaGetLastError();

        if (error != cudaSuccess)
        {
            std::cerr << "Kernel launch failed: "
                << cudaGetErrorString(error) << '\n';
            break;
        }

        // Wait for the GPU to finish executing the kernel.
        error = cudaDeviceSynchronize();

        if (error != cudaSuccess)
        {
            std::cerr << "Kernel execution failed: "
                << cudaGetErrorString(error) << '\n';
            break;
        }

        // Copy the result back to CPU memory.
        error = cudaMemcpy(
            &result,
            deviceValue,
            sizeof(int),
            cudaMemcpyDeviceToHost
        );

        if (error != cudaSuccess)
        {
            std::cerr << "Device-to-host copy failed: "
                << cudaGetErrorString(error) << '\n';
            break;
        }

        std::cout << "Initial value: " << hostValue << '\n';
        std::cout << "GPU result: " << result << '\n';

        success = (result == 42);

    } while (false);

    // Release GPU memory.
    error = cudaFree(deviceValue);

    if (error != cudaSuccess)
    {
        std::cerr << "cudaFree failed: "
            << cudaGetErrorString(error) << '\n';
        return false;
    }

    return success;
}