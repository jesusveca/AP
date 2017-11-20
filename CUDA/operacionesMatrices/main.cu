#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#define N 5
#define BLOCK_DIM 10
using namespace std;

__global__ 
void adicion_Matrices (int *a, int *b, int *c) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int fila = blockIdx.y * blockDim.y + threadIdx.y;
    int index = col + fila * N;
    if (col < N && fila < N) {
        c[index] = a[index] + b[index];
    }
}

__global__ 
void matrix_vector (int *a, int *b, int *c) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    if (col < N) {
        for(int i=0;i<N;i++){
            sum += b[i]*a[(i*N)+col];
        }
        c[col] = sum;
    }
}

__global__ 
void adicion_matrices_Filas (int *a, int *b, int *c) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int fila = blockIdx.y * blockDim.y + threadIdx.y;
    for(int i=col; i<N; i++){
        int index = i + fila * N;
        c[index] = a[index] + b[index];
    }
}

__global__ 
void adicion_matrices_Columnas (int *a, int *b, int *c) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int fila = blockIdx.y * blockDim.y + threadIdx.y;
    for(int i=fila; i<N; i++){
        int index = col + i * N;
        c[index] = a[index] + b[index];
    }
}

void imprimir(int matrix[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0; j<N; j++){
            std::cout << matrix[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}

void print_vector(int vector[N]){
    for(int j=0; j<N; j++){
        std::cout << vector[j] << '\t';
    }
}

void adicion_matrix_to_Kernel(){
    int a[N][N], b[N][N], c[N][N];
    int *dev_a, *dev_b, *dev_c;

    int size = N * N * sizeof(int);
    srand(time(NULL));
    for(int i=0; i<N; i++)
        for (int j=0; j<N; j++){
            a[i][j] = rand() % 3;
            b[i][j] = rand() % 3;
        }

    imprimir(a);
    std::cout << std::endl;
    imprimir(b);
    cudaMalloc((void**)&dev_a, size);
    cudaMalloc((void**)&dev_b, size);
    cudaMalloc((void**)&dev_c, size);

    cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);

    dim3 dimBlock(BLOCK_DIM, BLOCK_DIM);
    dim3 dimGrid((N+dimBlock.x-1)/dimBlock.x, (N+dimBlock.y-1)/dimBlock.y);

    adicion_Matrices<<<dimGrid,dimBlock>>>(dev_a,dev_b,dev_c);
    cudaDeviceSynchronize();

    cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);
    std::cout << std::endl;

    imprimir (c);

    /*
    for(int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            printf("%d\t", c[i][j] );
        }
        printf("\n");
    }
    */
}

int main() {
    int a[N][N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;

    int size = N * N * sizeof(int);
    srand(time(NULL));
    for(int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            a[i][j] = rand() % 9;
        }
        b[i] = rand() % 9;
    }

    imprimir(a);
    std::cout << std::endl;
    print_vector(b);

    cudaMalloc((void**)&dev_a, size);
    cudaMalloc((void**)&dev_b, N * sizeof(int));
    cudaMalloc((void**)&dev_c, N* sizeof(int));

    cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N*sizeof(int), cudaMemcpyHostToDevice);

    matrix_vector<<<N/256+1,256>>>(dev_a,dev_b,dev_c);

    cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);
    std::cout << std::endl;
    print_vector(c);

    return 0;
}