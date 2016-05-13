#include <iostream>

#include <cuda.h>

#include "util.h"

__host__
double f(double x) {
    return exp(cos(x))-2;
};

__host__
double fp(double x) {
    return -sin(x) * exp(cos(x));
};

// implements newton solve for
//      f(x) = 0
// where
//      f(x) = exp(cos(x)) - 2
void newton_host(int n, double *x) {
    for(int i=0; i<n; ++i) {
        auto x0 = x[i];
        for(int iter=0; iter<5; ++iter) {
            x0 -= f(x0)/fp(x0);
        }
        x[i] = x0;
    }
}

// TODO : implement newton_device() kernel that performs the work in newton_host
//        in parallel on the GPU

int main(int argc, char** argv) {
    size_t pow        = read_arg(argc, argv, 1, 20);

    size_t N = 1 << pow;
    auto size_in_bytes = N * sizeof(double);

    std::cout << "memory copy overlap test of length N = " << N
              << " : " << size_in_bytes/(1024.*1024.) << "MB"
              << std::endl;

    cuInit(0);

    double* xd = malloc_device<double>(N);
    double* xh = malloc_host<double>(N, 1.5);
    double* x  = malloc_host<double>(N);

    // compute kernel launch configuration
    auto block_dim = 128;
    auto grid_dim = (N+block_dim-1)/block_dim;

    auto time_h2d = -get_time();
    copy_to_device(xh, xd, N);
    time_h2d += get_time();

    cudaThreadSynchronize();
    auto time_kernel = -get_time();
    // TODO: launch kernel (use block_dim and grid_dim calculated above)
    cudaThreadSynchronize();
    time_kernel += get_time();

    auto time_d2h = -get_time();
    copy_to_host(xd, x, N);
    time_d2h += get_time();

    std::cout << "-------\ntimings\n-------" << std::endl;
    std::cout << "H2D    : " << time_h2d    << std::endl;
    std::cout << "D2H    : " << time_d2h    << std::endl;
    std::cout << "kernel : " << time_kernel << std::endl;

    // check for errors
    auto errors = 0;
    for(auto i=0; i<N; ++i) {
        if(std::fabs(f(x[i]))>1e-10) {
            errors++;
        }
    }
    if(errors>0) std::cout << "\n============ FAILED with " << errors << " errors" << std::endl;
    else         std::cout << "\n============ PASSED" << std::endl;

    cudaFree(xd);
    free(xh);
    free(x);

    return 0;
}

