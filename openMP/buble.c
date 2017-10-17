#include <stdio.h>
#include <stdlib.h>

void Bubble_sort(int a[], int n){
  int list_length, i, tmp;
  for(list_length = n; list_length >= 2; list_length--)
    for(i = 0; i < list_length-1; i++)
      if(a[i] > a[i+1]){
	tmp = a[i];
	a[i] = a[i+1];
	a[i+1] = tmp;
      }
}

int main(){
  int n, *A, i;
  scanf("%d", &n);
  A =  malloc(n*sizeof(int));
  for(i = 0; i < n; i++) scanf("%d", &A[i]);
  Bubble_sort(A,n);
  for(i = 0; i < n; i++) printf("%d ", A[i]);
  printf("\n");
  return 0;
}
