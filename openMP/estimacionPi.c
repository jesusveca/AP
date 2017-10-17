#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int numero_threads;

int steps = 100000000;
double step;

int main (int argc, const char *argv[]) {
    int i;
    double odd_even, acumula = 0.0;
    #pragma omp parallel for num_threads(numero_threads) \
        default(none) reduction(+:acumula) private(i, odd_even) \
        shared(steps) 
        for(i=0; i < steps; i++) {
            if (i % 2 == 0) odd_even = 1.0;
            else    odd_even =  -1.0;
            acumula += odd_even / (2 * i + 1);
    }
    printf("PI = %.16g\n", 4.0  * acumula);
}