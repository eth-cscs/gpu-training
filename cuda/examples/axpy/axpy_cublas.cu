#include <iostream>

#include <cuda.h>

#include "util.h"

int main(int argc, char** argv) {
    size_t N = read_arg(argc, argv, 1, 16);
    auto size_in_bytes = N * sizeof(double);

    std::cout << "memcopy and daxpy test of length N = " << N
              << " : " << size_in_bytes/(1024.*1024.) << "MB"
              << std::endl;

    cuInit(0);

    // initialize cublas
    auto cublas_handle = get_cublas_handle();

    double* x_device = malloc_device<double>(N);
    double* y_device = malloc_device<double>(N);

    double* x_host = malloc_host<double>(N, 1.5);
    double* y_host = malloc_host<double>(N, 3.0);
    double* y      = malloc_host<double>(N, 0.0);

    // copy to device
    auto start = get_time();
    copy_to_device<double>(x_host, x_device, N);
    copy_to_device<double>(y_host, y_device, N);
    auto time_H2D = get_time() - start;

    // y += 2 * x
    start = get_time();
    double alpha = 2.0;
    auto cublas_status =
        cublasDaxpy(cublas_handle, N, &alpha, x_device, 1, y_device, 1);
    cudaDeviceSynchronize();
    auto time_axpy = get_time() - start;

    // copy result back to host
    start = get_time();
    copy_to_host<double>(y_device, y, N);
    auto time_D2H = get_time() - start;

    std::cout << "-------\ntimings\n-------" << std::endl;
    std::cout << "H2D  : " << time_H2D << std::endl;
    std::cout << "D2H  : " << time_D2H << std::endl;
    std::cout << "axpy : " << time_axpy << std::endl;

    // check for errors
    auto errors = 0;
    #pragma omp parallel for reduction(+:errors)
    for(auto i=0; i<N; ++i) {
        if(std::fabs(6.-y[i])>1e-15) {
            errors++;
        }
    }

    if(errors>0) {
        std::cout << "\n============ FAILED with "
                  << errors << " errors" << std::endl;
    }
    else {
        std::cout << "\n============ PASSED" << std::endl;
    }

    cudaFree(x_device);
    cudaFree(y_device);

    free(x_host);
    free(y_host);
    free(y);

    return 0;
}

