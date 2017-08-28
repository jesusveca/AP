#include <stdio.h>
#include <time.h>

#define MAX 1000
double A[MAX][MAX], x[MAX], y[MAX];

int main()
{
    clock_t inicio1, fin1;
    double tiempoTotal1;

    inicio1 = clock();
    for(int i=0; i < MAX ; i++){
        for( int j=0; j < MAX ; j++){
            y[i] += A[i][j] * x[j];
        }
    }
    fin1 = clock();
    tiempoTotal1 = ((double) (fin1 - inicio1)) * 1000 / CLOCKS_PER_SEC;
    printf("%f\n", tiempoTotal1);
    return 0;
}