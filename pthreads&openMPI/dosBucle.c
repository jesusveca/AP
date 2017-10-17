#include <stdio.h>
#include <time.h>

#define MAX 1000
double A[MAX][MAX], x[MAX], y[MAX];

int main()
{
    clock_t inicio2,  fin2;
    double  tiempoTotal2;

    inicio2 = clock();

    for(int j=0; j < MAX ; j++){
        for( int i=0; i < MAX ; i++){
            y[i] += A[i][j] * x[j];
        }
    }

    fin2 = clock();
    tiempoTotal2 = ((double) (fin2 - inicio2)) * 1000 / CLOCKS_PER_SEC;

    printf("%f\n", tiempoTotal2);
    return 0;
}