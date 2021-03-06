#include <stdio.h>
#include <ctime>

__global__
void vecAddKernel(float *A, float *B, float *C, int n){
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if(i < n) C[i] = A[i] + B[i];
}

void vecAdd(float* A, float* B, float* C, int n){
  int size = n*sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMalloc((void **) &d_A, size);
  cudaMemcpy(d_A,A,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_B, size);
  cudaMemcpy(d_B,B,size,cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_C, size);

  vecAddKernel<<<ceil(n/1024.0), 1024>>>(d_A,d_B,d_C,n);
  
  cudaMemcpy(C,d_C,size,cudaMemcpyDeviceToHost);

  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}


int main(){
  unsigned t0,t1;
  int n,i;
  float *h_A,*h_B,*h_C;
  
  n=1000;
  h_A = (float*) malloc(n*sizeof(float));
  h_B = (float*) malloc(n*sizeof(float));
  h_C = (float*) malloc(n*sizeof(float));
  for(i = 0; i < n; i++){
    h_A[i] = i;
  }
  for(i = 0; i < n; i++){
    h_B[i] = i;
  }
  t0=clock();
  vecAdd(h_A,h_B,h_C,n);
  t1=clock();
  double time = (double(t1-t0)/CLOCKS_PER_SEC);
  for(i = 0; i < n; i++){
    printf("%f ", h_C[i]);
  }
  printf("Finalizado : %f\n: ",time);
  printf("\n");
  return 0;
}
