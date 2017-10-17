#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

int thread_count;

void Odd_even(int a[], int n){
  int phase, i, tmp;
  #pragma omp parallel num_threads(thread_count) default(none) shared(a,n) private(i,tmp,phase)
  for(phase = 0; phase < n; phase++){
    if(phase % 2 == 0)
      #pragma omp for
      for(i = 1; i < n; i+=2){
        tmp = a[i-1];
        a[i-1] = a[i];
        a[i] = tmp;
      }
    else
      #pragma omp for
      for(i = 1; i < n-1; i+=2){
        tmp = a[i+1];
        a[i+1] = a[i];
        a[i] = tmp;
      }
  }
}

int main(){
  thread_count = 16;
  int n, *A, i;

  
  n=2000;
  A =  malloc(n*sizeof(int));
  for(i = 0; i < n; i++) 
    A[i] = n-i-1;
  
  Odd_even(A,n);
  for(i = 0; i < n; i++) printf("%d ", A[i]);
  printf("\n");
  return 0;
}