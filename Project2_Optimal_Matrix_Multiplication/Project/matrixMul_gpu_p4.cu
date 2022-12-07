/*
 * _MATRIXMUL_GPU_CU_
 *
 * Orginal code by Mert SIDE
 * Updates by Stephen Devaney
 * 2022
 *
 * CS5375 Computer Systems Organization and Architecture 
 * Guest Lecture: GPU Programming
 *
 * Multiplying two matrices on the GPU
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------ GPUmatmul (unified memory) // *********************************************** added by Stephen Devaney in part 4
__global__
void init(int N, double *x, double *y, double *ans){
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      x[i*N+j] = 5;
      y[i*N+j] = (i==j?1:0);
      ans[i*N+j] = (double)0.000000000000;
    }
  }
}


// ------------------------------------------------------------------ GPUmatmul
__global__
void GPUmatmul(int N, double *x, double *y, double *ans){
  int t = threadIdx.x;  // thread number of a thread inside of a particular block *********************************************** added by Stephen Devaney in part 2
  int T = blockDim.x;  // total number of threads per block *********************************************** added by Stephen Devaney in part 2
  int b = blockIdx.x;  // block number of a block inside the grid *********************************************** added by Stephen Devaney in part 3
  int B = gridDim.x;  // total number of blocks per grid *********************************************** added by Stephen Devaney in part 3

  for(int i = b; i < N; i+=B){  // *********************************************** modified by Stephen Devaney in part 2 and 3
    for(int j = t; j < N; j+=T){
      for(int k = 0; k < N; k++){
        ans[i*N+j] += x[i*N+k] * y[k*N+j];
      }
    }
  }
}


// ---------------------------------------------------------------------- check
bool check(int N, double *ans){
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      if(ans[i*N+j] != 20.0) return false;
    }
  }
  return true;
}


// ----------------------------------------------------------------------- MAIN
int main(void){
  // size of matrix
  int N = 1<<9; // binary left-shift: 1 * 2^9 = 512
  printf("Size of matrix (N) is %d by %d.\n", N, N);
  int iter = 3;
  clock_t t;
  int blockSize = 128;  // number of threads per block *********************************************** added by Stephen Devaney in part 3
  int numBlocks = (N+blockSize-1) / blockSize;  // number of blocks *********************************************** added by Stephen Devaney in part 3
  
  // Martices
  double *x, *y, *ans;

  // Allocate Unified Memory - accessible from both CPU and GPU *********************************************** Added by Stephen Devaney in part 1
  cudaMallocManaged(&x, N * N * sizeof(double));
  cudaMallocManaged(&y, N * N * sizeof(double));
  cudaMallocManaged(&ans, N * N * sizeof(double));
  
  // ..........................................................................
  // Prefetch the data to the GPU
  int device = -1;
  cudaGetDevice(&device);
  cudaMemPrefetchAsync(x, N * N * sizeof(double), device, NULL);
  cudaMemPrefetchAsync(y, N * N * sizeof(double), device, NULL);
  cudaMemPrefetchAsync(ans, N * N * sizeof(double), device, NULL);
  
  // ..........................................................................
  // initialize x,y and ans arrays on the host
  init<<<numBlocks,blockSize>>>(N, x, y, ans);
  
  // ..........................................................................
  double avg=0;
  std::cout<<"Starting optimized GPU computation"<<std::endl;
  // Run kernel on GPU
  for(int i = 0; i <= iter; i++) {
    t = clock();
    GPUmatmul<<<numBlocks,blockSize>>>(N, x, y,ans); // *********************************************** updated by Stephen Devaney in part 2 and 3
    cudaDeviceSynchronize();
    t = clock() - t;
    if(i) avg += t; //we will ignore the first run
    // printf ("It took GPU-%d %f ms.\n",i,(((double)t)/CLOCKS_PER_SEC)*1000);
  }

  avg /= iter;
  avg /= CLOCKS_PER_SEC;
  avg *= 1000;
  printf("It took %lf ms on avg.\n", avg);
  if(check(N,ans)) std::cout<<"RUN OK."<<std::endl;
  else std::cout<<"RUN NOT OK."<<std::endl;

  // ..........................................................................
  
  // Free memory *********************************************** Added by Stephen Devaney in part 1
  cudaFree(x);
  cudaFree(y);
  cudaFree(ans);

  return 0;
}
/* EOF */
