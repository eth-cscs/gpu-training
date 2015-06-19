#pragma once

#include <cstdlib>

#include "omp.h"

///////////////////////////////////////////////////////////////////////////////
// everything below works both with gcc and nvcc
///////////////////////////////////////////////////////////////////////////////

// read command line arguments
// this is not very safe: undefined behaviour abounds if argv[index] can't
// be converted to an integer
static size_t read_arg(int argc, char** argv, size_t index, int default_value) {
    if(argc>index) {
        int n = atoi(argv[index]);
        if(n<0) {
            return default_value;
        }
        return n;
    }

    return default_value;
}

template <typename T>
T* malloc_host(size_t N, T value=T()) {
    T* ptr = (T*)(malloc(N*sizeof(T)));
    std::fill(ptr, ptr+N, value);

    return ptr;
}

double get_time() {
    return omp_get_wtime();
}
