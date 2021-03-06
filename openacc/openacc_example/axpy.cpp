#include <cmath>
#include <iostream>

#include "util.h"

void axpy(int n, double alpha, const double *x, double* y) {
    for(int i=0; i<n; ++i) {
        y[i] += alpha*x[i];
    }
}

int main(int argc, char** argv) {
    size_t pow = read_arg(argc, argv, 1, 16);
    size_t n = 1 << pow;
    size_t size_in_bytes = n * sizeof(double);

    std::cout << "memcopy and daxpy test of size " << n << std::endl;

    double* x = malloc_host<double>(n, 1.5);
    double* y = malloc_host<double>(n, 3.0);

    #ifdef FLUSH_CACHE
    // use dummy fields to avoid cache effects, which make results harder to interpret
    // use 1<<24 to ensure that cache is completely purged for all n
    double* x_ = malloc_host<double>(1<<24, 1.5);
    double* y_ = malloc_host<double>(1<<24, 3.0);
    axpy(1<<24, 2.0, x_, y_);
    #endif

    double start = get_time();
    axpy(n, 2.0, x, y);
    double time_axpy = get_time() - start;

    std::cout << "-------\ntimings\n-------" << std::endl;
    std::cout << "axpy : " << time_axpy << " s" << std::endl;
    std::cout << std::endl;

    // check for errors
    int errors = 0;
    for(int i=0; i<n; ++i) {
        if(std::fabs(6.-y[i])>1e-15) {
            errors++;
        }
    }

    if(errors>0) std::cout << "\n============ FAILED with " << errors << " errors" << std::endl;
    else         std::cout << "\n============ PASSED" << std::endl;

    free(x);
    free(y);

    return 0;
}
