#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>


int main(int argc, char* argv[]){
  int numero_threads = strtol(argv[1], NULL, 10);
  int m,n,i,j;
  double **Matrix, *vector_int, *vector_res;

  m=8000000; n=8;

  Matrix = (double**) malloc(m*sizeof(double));
  vector_int = (double*) malloc(n*sizeof(double));
  vector_res = (double*) malloc(m*sizeof(double));
  
  for(i = 0; i < m; i++) {
    Matrix[i] = (double*) malloc(n*sizeof(double));
    for(j = 0; j < n;j++){
      Matrix[i][j] = 1;
    }
  }

  double inicio = omp_get_wtime();

  #pragma omp parallel for num_threads(numero_threads) \
  default(none) private(i,j) \
  shared(Matrix,vector_int,vector_res,m,n)

  for(i = 0; i < m; i++){
    vector_res[i] = 0.0;
    for(j = 0; j < n; j++)
      vector_res[i] += Matrix[i][j] * vector_int[j];
  }
  printf("%f \n", omp_get_wtime()-inicio);
  return 0;
}