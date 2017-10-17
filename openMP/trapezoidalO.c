#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int numero_threads;

double funcion(double x) {
   return x*x;
}  

int main() {
   double  suma_g, inicio, final, x, pieces;     
   int     n, i;         

   numero_threads = 8;
   inicio=0;
   final=3;
   n=100;
   
   pieces = (final-inicio)/n;
   suma_g += (funcion(inicio) + funcion(final))/2.0;
   #pragma omp parallel for schedule(static) default(none) \
      shared(inicio, pieces, n) private(i, x) \
      reduction(+: suma_g) num_threads(numero_threads)
   for (i = 1; i <= n-1; i++) {
   		x = inicio + i * pieces;
   		suma_g += funcion(x);
   }
   suma_g = suma_g * pieces;
   printf("Resultado es %f \n",suma_g);
   return 0;
} 

