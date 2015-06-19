#include <cmath>
#include <iostream>

#include "util.h"

void axpy(int n, double alpha, const double *x, double* y) {
  int i;
#pragma acc parallel loop gang vector present(x[0:n],y[0:n]) firstprivate(alpha)
    for(i=0; i<n; ++i) {
        y[i] += alpha*x[i];
    }
}

int main(int argc, char** argv) {
    size_t pow = read_arg(argc, argv, 1, 16);
    size_t n = 1 << pow;
    size_t size_in_bytes = n * sizeof(double);
    double copyout_start,time_axpy,time_copyin;

    std::cout << "memcopy and daxpy test of size " << n << std::endl;

    double* x = malloc_host<double>(n, 1.5);
    double* y = malloc_host<double>(n, 3.0);

    //transfer the data to the accelerator memory
    double copyin_start = get_time();
#pragma acc data copyin(x[0:n]) copy(y[0:n])
    {
    time_copyin = get_time() - copyin_start;

    #ifdef FLUSH_CACHE
    // use dummy fields to avoid cache effects, which make results harder to interpret
    // use 1<<24 to ensure that cache is completely purged for all n
    double* x_ = malloc_host<double>(1<<24, 1.5);
    double* y_ = malloc_host<double>(1<<24, 3.0);
#pragma acc data copyin(x_[0:1<<24]) copy(y_[0:1<<24])
    { 
      axpy(1<<24, 2.0, x_, y_);
    }
    #endif

    double start = get_time();

    axpy(n, 2.0, x, y);

    time_axpy = get_time() - start;
    copyout_start = get_time();
    }
    double time_copyout = get_time() - copyout_start;

    std::cout << "-------\ntimings\n-------" << std::endl;
    std::cout << "axpy    : " << time_axpy    << " s" << std::endl;
    std::cout << "copyin  : " << time_copyin  << " s" << std::endl;
    std::cout << "copyout : " << time_copyout << " s" << std::endl;
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
