#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int     numThreads; // variables globales compartidas para los threads
double  a, b, h;
int     n, local_n;

pthread_mutex_t   mutex;
double  total;
double funcion(double x) {
    double funcionEvaluar;
    funcionEvaluar = x*x;
    return funcionEvaluar;
}

double trapezoidalFun(double  local_a, double  local_b, int local_n, double  h) {
    double integral;   double x;    int i;

    integral = (funcion(local_a) + funcion(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        integral += funcion(x);
    }
    integral = integral*h;
    return integral;
} 

void *Thread_work(void* rank) {
    double  local_a;   double local_b;   
    double  my_int;    long my_rank = (long) rank;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = trapezoidalFun(local_a, local_b, local_n, h);

    pthread_mutex_lock(&mutex);
    total += my_int;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char** argv) {
    long        i;
    pthread_t*  arrThreads;  
    total = 0.0;

    numThreads = strtol(argv[1], NULL, 10);
    printf("Ingrese inicio, fin, numPedazos: \n");
    scanf("%lf %lf %d", &a, &b, &n);
    h = (b-a)/n;
    local_n = n/numThreads;

    arrThreads = malloc (numThreads*sizeof(pthread_t)); // Asignar almacenamiento para id threads

    pthread_mutex_init(&mutex, NULL); 

    for (i = 0; i < numThreads; i++)    pthread_create(&arrThreads[i], NULL, Thread_work, (void*) i);
    for (i = 0; i < numThreads; i++)    pthread_join(arrThreads[i], NULL);

    printf("Con %d pedazos ",n); printf(" desde %f hasta %f es:  %19.15e\n",a, b, total);
    pthread_mutex_destroy(&mutex);
    free(arrThreads);
    return 0;
}
