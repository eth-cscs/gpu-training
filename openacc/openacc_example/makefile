# gnu compiler
axpy.gcc : axpy.cpp util.h
	g++ -fopenacc -O3 axpy.cpp -o axpy.gcc

# Cray compiler

# C version
axpy : axpy.cpp util.h
	CC -h omp -h acc -O3 axpy.cpp -o axpy

axpy_openacc : axpy_openacc.cpp util.h
	CC -h omp -h acc  -hlist=m -O3 axpy_openacc.cpp -o axpy_openacc

# fortran version
axpy.fort : axpy.f90 util.f03
	ftn -h omp -h acc  -rm -O3 -c util.f03
	ftn -h omp -h acc  -rm -O3 -c axpy.f90
	ftn -h omp -h acc  -rm -O3 axpy.o util.o -o axpy.fort

axpy_openacc.fort : axpy_openacc.f90 util.f03
	ftn -h omp -h acc  -rm -O3 -c util.f03
	ftn -h omp -h acc  -rm -O3 -c axpy_openacc.f90
	ftn -h omp -h acc  -rm -O3 axpy_openacc.o util.o -o axpy_openacc.fort

clean :
	rm -f *.o *.mod axpy axpy.gcc axpy_openacc axpy_openacc.fort
