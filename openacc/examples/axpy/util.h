#pragma once

#ifdef PGI
#   include <cstdlib>
#   include <omp.h>
#   include <sstream>
#else
#   include <chrono>
#endif
#include <cmath>

#ifndef NO_CUDA
#include <cuda.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>


// helper for initializing cublas
// use only for demos: not threadsafe
static cublasHandle_t get_cublas_handle() {
    static bool is_initialized = false;
    static cublasHandle_t cublas_handle;

    if(!is_initialized) {
        cublasCreate(&cublas_handle);
    }
    return cublas_handle;
}

///////////////////////////////////////////////////////////////////////////////
// CUDA error checking
///////////////////////////////////////////////////////////////////////////////
static void cuda_check_status(cudaError_t status) {
    if(status != cudaSuccess) {
        std::cerr << "error: CUDA API call : "
                  << cudaGetErrorString(status) << std::endl;
        exit(1);
    }
}

static void cuda_check_last_kernel(std::string const& errstr) {
    auto status = cudaGetLastError();
    if(status != cudaSuccess) {
        std::cout << "error: CUDA kernel launch : " << errstr << " : "
                  << cudaGetErrorString(status) << std::endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////////
// allocating memory
///////////////////////////////////////////////////////////////////////////////

// allocate space on GPU for n instances of type T
template <typename T>
T* malloc_device(size_t n) {
    void* p;
    auto status = cudaMalloc(&p, n*sizeof(T));
    cuda_check_status(status);
    return (T*)p;
}

// allocate page-locked host memory for n instances of type T
template <typename T>
T* malloc_host_pinned(size_t N, T value=T()) {
    T* ptr = nullptr;
    cudaHostAlloc((void**)&ptr, N*sizeof(T), 0);

    std::fill(ptr, ptr+N, value);

    return ptr;
}

///////////////////////////////////////////////////////////////////////////////
// copying memory
///////////////////////////////////////////////////////////////////////////////

// copy n*T from host to device
// If a cuda stream is passed as the final argument the copy will be performed
// asynchronously in the specified stream, otherwise it will be serialized in
// the default (NULL) stream
template <typename T>
void copy_to_device_async(const T* from, T* to, size_t n, cudaStream_t stream=NULL) {
    auto status =
        cudaMemcpyAsync(to, from, n*sizeof(T), cudaMemcpyHostToDevice, stream);
    cuda_check_status(status);
}

// copy n*T from device to host
// If a cuda stream is passed as the final argument the copy will be performed
// asynchronously in the specified stream, otherwise it will be serialized in
// the default (NULL) stream
template <typename T>
void copy_to_host_async(const T* from, T* to, size_t n, cudaStream_t stream=NULL) {
    auto status =
        cudaMemcpyAsync(to, from, n*sizeof(T), cudaMemcpyDeviceToHost, stream);
    cuda_check_status(status);
}

// copy n*T from host to device
template <typename T>
void copy_to_device(T* from, T* to, size_t n) {
    auto status =
        cudaMemcpy(to, from, n*sizeof(T), cudaMemcpyHostToDevice);
    cuda_check_status(status);
}

// copy n*T from device to host
template <typename T>
void copy_to_host(T* from, T* to, size_t n) {
    auto status =
        cudaMemcpy(to, from, n*sizeof(T), cudaMemcpyDeviceToHost);
    cuda_check_status(status);
}

#endif
///////////////////////////////////////////////////////////////////////////////
// everything below works both with gcc and nvcc
///////////////////////////////////////////////////////////////////////////////

// read command line arguments
static size_t read_arg(int argc, char** argv, size_t index, int default_value) {
    if(argc>index) {
        try {
#ifdef PGI
            std::stringstream arg_n(argv[index]);
            int n;
            arg_n >> n;
#else
            auto n = std::stoi(argv[index]);
#endif
            if(n<0) {
                return default_value;
            }
            return n;
        }
        catch (std::exception e) {
            std::cout << "error : invalid argument \'" << argv[index]
                      << "\', expected a positive integer." << std::endl;
            exit(1);
        }
    }

    return default_value;
}

template <typename T>
T* malloc_host(size_t N, T value=T()) {
    T* ptr = (T*)(malloc(N*sizeof(T)));
    std::fill(ptr, ptr+N, value);

    return ptr;
}

#ifdef PGI
static double get_time()
{
    return omp_get_wtime();
}

#else
// aliases for types used in timing host code
using clock_type    = std::chrono::high_resolution_clock;
using duration_type = std::chrono::duration<double>;

// return the time in seconds since the get_time function was first called
// for demos only: not threadsafe
static double get_time() {
    static auto start_time = clock_type::now();
    return duration_type(clock_type::now()-start_time).count();
}

#endif
