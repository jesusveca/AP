#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ 
void adicion_vectores(float x[], float y[], float z[], int n) {
   int idx = blockDim.x * blockIdx.x + threadIdx.x;
   if (idx < n) 
    z[idx] = x[idx] + y[idx];
}  

int main(int argc, char* argv[]) {
   int n, i;
   float *host_x, *host_y, *host_z, *device_x, *device_y, *device_z;
   int threads_bloque, tamanoBloque;
   size_t size;
   n=10;
   size = n*sizeof(float);
   host_x = (float*) malloc(size);      host_y = (float*) malloc(size);     host_z = (float*) malloc(size);

   for (i = 0; i < n; i++) {
      host_x[i] = i+10;      host_y[i] = n-3;
   }

   cudaMalloc(&device_x, size);
   cudaMalloc(&device_y, size);
   cudaMalloc(&device_z, size);

   cudaMemcpy(device_x, host_x, size, cudaMemcpyHostToDevice);
   cudaMemcpy(device_y, host_y, size, cudaMemcpyHostToDevice);

   threads_bloque = 256;

   tamanoBloque = (n + threads_bloque - 1)/threads_bloque;
   adicion_vectores<<<tamanoBloque, threads_bloque>>>(device_x, device_y, device_z, n);

   cudaThreadSynchronize();

   cudaMemcpy(host_z, device_z, size, cudaMemcpyDeviceToHost);
   printf("La suma es: \n");
   for (i = 0; i < n; i++)
      printf("%.1f ", host_z[i]);
   printf("\n");
   return 0;
}  