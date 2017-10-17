#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

int thread_count;


// two parallel directive
void odd_even_sort_first(int a[], int n){
  int phase, i, tmp;
  for(phase = 0; phase < n; phase++){
    if(phase % 2 == 0)
    #pragma omp parallel for num_threads(thread_count) default(none) shared(a,n) private(i,tmp)
      for(i = 1; i < n; i+=2){
        if(a[i-1] > a[i]){
          tmp = a[i-1];     a[i-1] = a[i];      a[i] = tmp;
        }
      }
    else
    #pragma omp parallel for num_threads(thread_count) default(none) shared(a,n) private(i,tmp)
      for(i = 1; i < n-1; i+=2){
        if(a[i-1] > a[i]){
          tmp = a[i-1];     a[i-1] = a[i];      a[i] = tmp;
        }
      }
  }
}


// two for
void odd_even_sort_second(int vector_int[], int numero_elem){
  int phase, iter, tempo;

  #pragma omp parallel num_threads(thread_count) default(none) shared(vector_int,numero_elem) private(iter,tempo,phase)
  
  for(phase = 0; phase < numero_elem; phase++){
    if(phase % 2 == 0) 
      #pragma omp for
      for(iter = 1; iter < numero_elem; iter+=2){
        if(vector_int[iter-1] > vector_int[iter]){
          tempo = vector_int[iter-1];     vector_int[iter-1] = vector_int[iter];      vector_int[iter] = tempo;
        }
      }
    else
      #pragma omp for
      for(iter = 1; iter < numero_elem-1; iter+=2){
        if(vector_int[iter] > vector_int[iter+1]){
          tempo = vector_int[iter+1];     vector_int[iter+1] = vector_int[iter];      vector_int[iter] = tempo;
        }
      }
  }
}

int main(int argc, char* argv[]){
  thread_count = strtol(argv[1], NULL, 10);
  int n, *A, i;

  n=20000;
  A =  malloc(n*sizeof(int));
  for(i = 0; i < n; i++) A[i] = n-i-1;
  
  double inicio = omp_get_wtime();
  odd_even_sort_first(A,n);
  printf("%f \n", omp_get_wtime()-inicio);

  inicio = omp_get_wtime();
  odd_even_sort_second(A,n);
  printf("%f \n", omp_get_wtime()-inicio);

  return 0;
}