#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define min(x, y) (((x) < (y)) ? (x) : (y))

int main(){
    int i, j, k, a, b, c, ii, jj, kk, tamanoBloque = 2;
    //scanf("%d %d %d", &a, &b, &c);
    a=b=c=1000;
    int **matriz1 = malloc(sizeof *matriz1 * a);
   	int **matriz2 = malloc(sizeof *matriz2 * b);
	int **result = malloc(sizeof *result * a);
    for(i = 0; i < a; i++)
		matriz1[i] = malloc(sizeof * matriz1[i]*b);
	for(i = 0; i < b; i++)
        matriz2[i] = malloc(sizeof * matriz2[i]*c);
	for(i = 0; i < a; i++)
		result[i] = malloc(sizeof * result[i]*c);
    for(i = 0; i < a; i++)
        for(j = 0; j < b; j++)
            matriz1[i][j] = 1;
    for(i = 0; i < b; i++)
        for(j = 0; j < c; j++)
            matriz2[i][j] = 1;
	for(i = 0; i < a; i++)
        for(j = 0; j < c; j++)
            result[i][j] = 0;
    clock_t inicio = clock();
    for(ii = 0; ii < a; ii+=tamanoBloque)
        for(jj = 0; jj < c; jj += tamanoBloque)
            for(kk = 0; kk < b; kk += tamanoBloque)
                for(i = ii; i < min(ii+tamanoBloque,a); i++)
                    for(j = jj; j < min(jj+tamanoBloque,c); j++)
                        for(k = kk; k < min(kk+tamanoBloque,b); k++)
                            result[i][j] += (matriz1[i][k] * matriz2[k][j]);
    clock_t fin = clock();
    double tiempoTotal;
    tiempoTotal = (double)(fin-inicio) /CLOCKS_PER_SEC;
    printf("%f\n", tiempoTotal);
    return 0;
}