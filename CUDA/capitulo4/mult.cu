#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>



#include <stdio.h>
#include <stdlib.h>

#define N 20
#define BLOCK_DIM 10

using namespace std;
#define TILE_WIDTH 4


/*
// EJERCICIO DE LA SECCION 4.7
__global__
void matMultKernel_tile_siete_siente(int *d_M, int *d_N, int *d_P, int Width){

  extern __shared__ int Mds[][];
  extern __shared__ int Nds[][];

  int bx = blockIdx.x; int by = blockIdx.y;
  int tx = threadIdx.x; int ty = threadIdx.y;
 
  int Row = by*TILE_WIDTH + ty;
  int Col = bx*TILE_WIDTH + tx;

  float Pvalue = 0;
  int  ph,k;
  for(ph = 0; ph < Width/TILE_WIDTH; ++ph){
    // Collaborative loading of M and N tiles into shared memory
    if ( (Row<Width) && (ph*TILE_WIDTH+tx)<Width )
        Mds[ty][tx] = d_M[Row*Width + ph*TILE_WIDTH + tx];
    if ( (ph*TILE_WIDTH+ty)<Width && Col<Width )
        Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty) + Col];
    
    __syncthreads();
    for(k = 0; k < TILE_WIDTH; ++k){
      Pvalue += Mds[ty][k] * Nds[k][tx];
     }
     __syncthreads();
  }
  if ( (Row<Width) && (Col<Width))
    d_P[Row*Width + Col] = Pvalue;
}

*/

// EJERCICIO DE LA SECCION 4.6
__global__
void matMultKernel_tile_seis(int *d_M, int *d_N, int *d_P, int Width){

  __shared__ int Mds[TILE_WIDTH][TILE_WIDTH];
  __shared__ int Nds[TILE_WIDTH][TILE_WIDTH];

  int bx = blockIdx.x; int by = blockIdx.y;
  int tx = threadIdx.x; int ty = threadIdx.y;
 
  int Row = by*TILE_WIDTH + ty;
  int Col = bx*TILE_WIDTH + tx;

  float Pvalue = 0;
  int  ph,k;
  for(ph = 0; ph < Width/TILE_WIDTH; ++ph){
    // Collaborative loading of M and N tiles into shared memory
    if ( (Row<Width) && (ph*TILE_WIDTH+tx)<Width )
        Mds[ty][tx] = d_M[Row*Width + ph*TILE_WIDTH + tx];
    if ( (ph*TILE_WIDTH+ty)<Width && Col<Width )
        Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty) + Col];
    
    __syncthreads();
    for(k = 0; k < TILE_WIDTH; ++k){
      Pvalue += Mds[ty][k] * Nds[k][tx];
     }
     __syncthreads();
  }
  if ( (Row<Width) && (Col<Width))
    d_P[Row*Width + Col] = Pvalue;
}



// EJERCICIO DE LA SECCION 4.4
__global__
void matMultKernel_tile(int *d_M, int *d_N, int *d_P, int Width){

  __shared__ int Mds[TILE_WIDTH][TILE_WIDTH];
  __shared__ int Nds[TILE_WIDTH][TILE_WIDTH];

  int bx = blockIdx.x; int by = blockIdx.y;
  int tx = threadIdx.x; int ty = threadIdx.y;
 
  int Row = by*TILE_WIDTH + ty;
  int Col = bx*TILE_WIDTH + tx;

  float Pvalue = 0;
  int  ph,k;
  for(ph = 0; ph < Width/TILE_WIDTH; ++ph){
     Mds[ty][tx] = d_M[Row*Width+ph*TILE_WIDTH + tx];
     Nds[ty][tx] = d_N[(ph*TILE_WIDTH + ty) * Width + Col];
     __syncthreads();
     for(k = 0; k < TILE_WIDTH; ++k){
      Pvalue += Mds[ty][k] * Nds[k][tx];
     }
     __syncthreads();
  }
  d_P[Row*Width + Col] = Pvalue;
}

// EJERCICIO DE LA SECCION 4.2

__global__
void matMultKernel(int *d_M, int *d_N, int *d_P, int Width){
  int Row = blockIdx.y*blockDim.y + threadIdx.y;
  int Col = blockIdx.x*blockDim.x + threadIdx.x;
  int k = 0;
  if(Row < Width && Col < Width){
      float Pvalue = 0;
      for(k = 0; k < Width; ++k){
          Pvalue += d_M[Row*Width + k] * d_N[k*Width+Col];
      }
      d_P[Row*Width+Col] = Pvalue;
  }
}

__global__ 
void sum_Matrices_Normal (int *a, int *b, int *c) {
    int columna = blockIdx.x * blockDim.x + threadIdx.x;
    int fila = blockIdx.y * blockDim.y + threadIdx.y;
    int id = columna + fila * N;
    if (columna < N && fila < N) {
        c[id] = a[id] + b[id];
    }

}

void imprimir_Matriz(int matrix[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0; j<N; j++){
            cout<<matrix[i][j]<<' ';
        }
        cout<<endl;
    }
}

void inicio_matrices(){
    int a[N][N], b[N][N], c[N][N];
    int *dev_a, *dev_b, *dev_c;

    int size = N * N * sizeof(int);
    srand(time(NULL));
    for(int i=0; i<N; i++)
        for (int j=0; j<N; j++){
            a[i][j] = 1;
            b[i][j] = 1;
        }

    imprimir_Matriz(a);
    cout<<endl;
    imprimir_Matriz(b);


    cudaMalloc((void**)&dev_a, size);
    cudaMalloc((void**)&dev_b, size);
    cudaMalloc((void**)&dev_c, size);
    cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

    //dim3 dimBlock(N,N); // cuantos threads se ejecutaran juntos y que compartiran memoria en un sigle proccessor
    //dim3 dimGrid(1,1); // un grupo de thread block que se ejecutan en un sigle cuda program logically in parallel
    
    //dim3 dimGrid(ceil(N/1024.0),ceil(N/1024.0),1);
    //dim3 dimBlock(1024,1024,1);  /*Dimensiónde 2 X 2, eje Z desactivado con 1*/
    
    
    dim3 dimGrid(ceil(N/4.0),ceil(N/4.0),1);
    dim3 dimBlock(TILE_WIDTH,TILE_WIDTH,1);
    matMultKernel_tile_seis<<<dimGrid,dimBlock>>>(dev_a,dev_b,dev_c, N);
    
    
    //for siete
    //size_t size=  calculate_appropriate_SM_usage(dev_prop.sharedMemPerBlock,...);
    //matMultKernel_tile_siete_siente<<<dimGrid, dimBlock, size>>>(Md, Nd, Pd, Width);
    
    
    
    cudaDeviceSynchronize();
    cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);
    
    cout<<endl;
    for(int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            printf("%d ", c[i][j] );
        }
        printf("\n");
    }

    cudaFree(dev_a);    cudaFree(dev_b);    cudaFree(dev_c);
}

int main() {
    inicio_matrices();

    return 0;
}