# Matrix Multiplication Kernel
Texas Tech University, CS5375 Computer Systems Organization and Architecture, Project**
* Stephen Devaney
  * R11549471
  * stephen.c.devaney@ttu.edu

## Description
This project uses a matrix multiplication kernel to provide a basic understanding of parallel comuting using CUDA.
The matrix multiplicion kernel consist of two N x N matrices. This project consists of 5 files and a description 
of each can be found below. The base level code was provided by for TTU CS5375 course by Mert Side, Ghazanfar ALI, 
and Yong CHEN. The orginal repo can be found [here](https://github.com/mertside/CS5375_GPU_Lecture).

## Files
### matrixMul_cpu.cpp
This file is the serial CPU version of the kernal. Does not require cuda to run.
* To build the file (2 ways):
  * Using provided Makefile: make matrixMul_cpu
  * Using the command line: g++ matrixMul_cpu.cpp -o matrixMul_cpu.exe
* To run the file (linux):
  * Without time command: ./matrixMul_cpu.exe
  * With time command: time ./matrixMul_cpu.exe
  
### matrixMul_gpu_p1.cu
This file is the serial GPU version of the kernal. Added memory allocations and free for CUDA from first file. Does require cuda to run.
* To build the file (2 ways):
  * Using provided Makefile: make matrixMul_gpu_p1
  * Using the command line: nvcc matrixMul_gpu_p1.cu -o matrixMul_gpu_p1.exe
* To run the file (linux):
  * Without profiler: ./matrixMul_gpu_p1.exe
  * With profiler: nvprof  ./matrixMul_gpu_p1.exe
  
### matrixMul_gpu_p2.cu
This file addes threads to matrixMul_gpu_p1.cu and is the start of parallelization. Added threadIdx, blockDim. Modified GPUmatmul function call to be from device. Modfied Loop iterating through the matricies so that each cell is assigned to different threads. Does require cuda to run.
* To build the file (2 ways):
  * Using provided Makefile: make matrixMul_gpu_p2
  * Using the command line: nvcc matrixMul_gpu_p2.cu -o matrixMul_gpu_p2.exe
* To run the file (linux):
  * Without profiler: ./matrixMul_gpu_p2.exe
  * With profiler: nvprof  ./matrixMul_gpu_p2.exe
  
### matrixMul_gpu_p3.cu
This file addes blocks to matrixMul_gpu_p2.cu. Added blockIdx, gridDim, AC, index, blockSize, numBlocks. Modified GPUmatmul function call to include block size. Modfied Loop iterating through the matricies so that each cell is assigned to different threads in all blocks. Does require cuda to run.
* To build the file (2 ways):
  * Using provided Makefile: make matrixMul_gpu_p3
  * Using the command line: nvcc matrixMul_gpu_p3.cu -o matrixMul_gpu_p3.exe
* To run the file (linux):
  * Without profiler: ./matrixMul_gpu_p3.exe
  * With profiler: nvprof  ./matrixMul_gpu_p3.exe
  
### matrixMul_gpu_p4.cu
This file is provides optimization to matrixMul_gpu_p2.cu by modifing the number of threads, using unified memory initization, and using prefetching. Created init function and move initiation into this function. Added prefetch calls. Change # thread from 256 to 128. Does require cuda to run.
* To build the file (2 ways):
  * Using provided Makefile: make matrixMul_gpu_p4
  * Using the command line: nvcc matrixMul_gpu_p4.cu -o matrixMul_gpu_p4.exe
* To run the file (linux):
  * Without profiler: ./matrixMul_gpu_p4.exe
  * With profiler: nvprof  ./matrixMul_gpu_p4.exe
  
### submit_job.sh
Bash script file to submit jobs on a system utlizing SLURM. Variables in the top of the file can be change to true in order to run different version of the Matrix Multiplicaion Kernel. This file will build all versions that the user defined using the provided Makefile. Makefile is require for this submission script.
* Usage: sbatch submit_job.sh

### Makefile
Provide make file to build all kernel versions. Usage is define above in kernel version descriptons.
