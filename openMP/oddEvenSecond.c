#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

int numero_threads;

void odd_even_sort_second(int vector_int[], int numero_elem){
  int phase, iter, tempo;

  #pragma omp parallel num_threads(numero_threads) \
    default(none) shared(vector_int,numero_elem) \
    private(iter,tempo,phase)
  
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
  numero_threads = strtol(argv[1], NULL, 10);;
  int numero_elem, *vector_int, iter;  
  numero_elem=100000;
  vector_int =  malloc(numero_elem*sizeof(int));

  for(iter = 0; iter < numero_elem; iter++)  // fill array
    vector_int[iter] = numero_elem-iter-1;
  
  double inicio = omp_get_wtime();
  
  odd_even_sort_second(vector_int,numero_elem);

  printf("%f \n", omp_get_wtime()-inicio);
  //for(iter = 0; iter < numero_elem; iter++) printf("%d ", vector_int[iter]);  printf("\numero_elem");
  // double inicio = omp_get_wtime();
  return 0;
}