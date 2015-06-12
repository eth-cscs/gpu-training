# CSCS GPU Training

## Requirements

### Laptops
Participants must bring their own laptop to do the exercises. CSCS will provide guest accounts and a reservation on our system Piz Daint, which has GPUs and the tools (compilers, profilers, etc.) that will be required.

To use Piz Daint, you have to be able to log onto our system using ssh:
* Mac and Linux: easy with the built in terminal!
* Windows: users who don't know how to log into a remote system using ssh should contact us before hand to get set up.

Users who have a GPU in their laptops will be able to do the CUDA part of the course on their laptop, if they have CUDA 6.5 and a C++11 capable compiler.

The OpenACC part of the course requires an OpenACC compiler, for which remote access to Daint will be required.

The pratical sessions will also require that you can edit text files, for which you need to use a text editor. It is recommended that people use emacs or vim if they know how. Otherwise gedit is also available, for participants who have x-windows set up.

Please contact us beforehand if the previous paragraph didn't make any sense!

### Programming Skills
We assume that participants know how to program.

CUDA is a dialect of C++ (it is C++ with some additional keywords and syntax for GPU-specific programming). A reasonable level of experience with C/C++ is required to do all of the CUDA practical sessions. You will still be able to follow the presentation, however there will be a strong focus on the practical component. We will try to get Fortran programmers to team up with a C++ programmer at the start.

Some basic C++/C++11 features will be used, for which a short tutorial/primer will be posted on this wiki before the course so that participants can be prepared.

OpenACC is a set of extensions to both C/C++ and Fortran. So users of both languages will be able to choose. However, again a solid understanding of one of the two languages is required.

## Schedule

The start and end times on each day are adjusted to give people who want to travel to/from Lugano the time to do so.

- Thursday : 10:00 am - 5:30 pm
- Friday   :  8:30 am - 4:00 pm

Note that there will be coffee breaks and practical sessions in the schedule below.

### Thursday

- 10:00 - 12:00  Introduction to GPUs and CUDA
- 12:00 - 12:30  Lunch
- 12:30 - 17:30  Introduction CUDA

### Friday

-  8:30 - 10:00  Introduction to CUDA
- 10:15 - 12:00  Introduction to OpenACC
- 12:00 - 12:30  Lunch
- 12:30 - 16:00  Introduciton to OpenACC

## Topics

### Introduction to CUDA

Each section, except for the introduction to GPU computing, will involve hands on practical where participants will get to try out the material covered in the lecture material.

1. __Introduction to GPU computing__. A brief overview of why people are turning to accelerator architectures like GPUs instead of sticking to the tried-and-tested multicore processors that have served us well over the last couple of decades. Also an introduction to GPU architecture and programming models.
2. __Managing GPU Memory and API__. Overview of the memory model of GPUs, and an introduction to the runtime library functions provided by CUDA for managing memory. The practical will see the students work on some useful helper functiosn that will be used in later practical exercises.
3. __Writing GPU Kernels__. Discussion of parallelism on the GPU, and introduction of how to write and launch a parallel kernel in CUDA. The practical will involve writing a parallel vector adition kernel.
4. __Scaling Up with Thread Blocks__. Using thread blocks to utilize all resources on the GPU. For the practical the participants will extend the vector addition kernel to take advantage of massive parallelism.
5. __Coordination Between Threads__. Overview of mechanisms provided by CUDA for getting threads to communicate and coordinate. Practical examples with shared memory include pipelined stencils and a dot product.
6. __Concurrent Code__. Review of the different opportunities for task-based paralellism, include concurrent kernels, memory transactions and host-device concurrency. The practicals include streaming workload implementation.


### Introduction to OpenACC

1. __Overview of the OpenACC programming paradigm__. What are the differences between CUDA and OpenACC, what are the strengths/weaknesses and what kind of development is OpenACC well suited for? Overview of the OpenACC execution model.
2. __OpenACC directives__. Syntax, conditional compilation. What classes of directives exist and how are they used.
3. __Data scoping__. How to manage data access of the GPU threads to objects in device memory.
4. __Data transfers__. How to handle data transfers between host and device and how to optimize these.
5. __Kernels__. How to write kernels and some options for performance tuning.
6. __Reduction operations__. How to use reductions in OpenACC and some caveats about synchronisation. 
7. __CUDA interoperability__. How to call CUDA kernels from OpenACC code and use data already present on the device.
8. __Asynchronous operations in OpenACC__. How to overlap different operations on the accelerator and transfers between host and device memory? 
