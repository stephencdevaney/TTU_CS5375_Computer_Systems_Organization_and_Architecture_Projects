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

// ------------------------------------------------------------------ GPUmatmul
__global__
void GPUmatmul(int N, double *x, double *y, double *ans){
  int t = threadIdx.x;  // thread number of a thread inside of a particular block *********************************************** added by Stephen Devaney in part 2
  int T = blockDim.x;  // total number of threads per block *********************************************** added by Stephen Devaney in part 2
  for(int i = t; i < N * N; i+=T){
      for(int j = 0; j < N; j++){
          ans[i] += x[i/N+j] * y[i/N+j*N];
      }
  }
//  for(int i = t; i < N * N; i+=T){
//    for(int j = 0; j < N; j++) {
//      for(int k = 0; k < N; k++) {
//        ans[i*N+j] += (x[i*N+k] * y[k*N+j]);
//      }
//    }
//  }
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
  
  // Martices
  double *x, *y, *ans;

  // Allocate Unified Memory - accessible from both CPU and GPU *********************************************** Added by Stephen Devaney in part 1
  cudaMallocManaged(&x, N * N * sizeof(double));
  cudaMallocManaged(&y, N * N * sizeof(double));
  cudaMallocManaged(&ans, N * N * sizeof(double));

  // ..........................................................................
  // initialize x,y and ans arrays on the host
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      x[i*N+j] = 5;
      y[i*N+j] = (i==j?1:0);
      ans[i*N+j] = (double)0.000000000000;
    }
  }

  // ..........................................................................
  double avg=0;
  std::cout<<"Starting unoptimized GPU computation"<<std::endl;
  // Run kernel on GPU
  for(int i = 0; i <= iter; i++) {
    t = clock();
    GPUmatmul<<<1,256>>>(N, x, y,ans); // *********************************************** updated by Stephen Devaney in part 2
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